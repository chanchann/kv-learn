// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_DB_VERSION_EDIT_H_
#define STORAGE_LEVELDB_DB_VERSION_EDIT_H_

#include <set>
#include <utility>
#include <vector>

#include "db/dbformat.h"

namespace leveldb {

class VersionSet;

// 文件的一些元信息
struct FileMetaData {
  FileMetaData() : refs(0), allowed_seeks(1 << 30), file_size(0) {}

  int refs;
  int allowed_seeks;  // Seeks allowed until compaction
  // 文件编号
  uint64_t number;
  uint64_t file_size;    // File size in bytes
  InternalKey smallest;  // Smallest internal key served by table
  InternalKey largest;   // Largest internal key served by table
};

// VersionEdit 是一个用于描述版本变更的类，它的主要作用是记录版本变更的操作，包括添加文件、删除文件、设置文件指针等。
// 在 LevelDB 中，每个版本都对应着一个 VersionEdit 对象，当需要对某个版本进行变更时，就需要创建一个 VersionEdit 对象，并将变更操作记录到该对象中。
// 在 VersionSet::LogAndApply 函数中，会将 VersionEdit 对象序列化成字符串，写入到 MANIFEST 文件中，以便在重启时能够恢复出最新的版本信息。
// 
// VersionEdit通过两个数组new_files_和deleted_files_来保存针对当前Version要增加和删除的文件
// 通过AddFile()和DeleteFile()两个操作来实现
class VersionEdit {
 public:
  VersionEdit() { Clear(); }
  ~VersionEdit() = default;

  void Clear();

  void SetComparatorName(const Slice& name) {
    has_comparator_ = true;
    comparator_ = name.ToString();
  }
  void SetLogNumber(uint64_t num) {
    has_log_number_ = true;
    log_number_ = num;
  }
  void SetPrevLogNumber(uint64_t num) {
    has_prev_log_number_ = true;
    prev_log_number_ = num;
  }
  void SetNextFile(uint64_t num) {
    has_next_file_number_ = true;
    next_file_number_ = num;
  }
  void SetLastSequence(SequenceNumber seq) {
    has_last_sequence_ = true;
    last_sequence_ = seq;
  }
  void SetCompactPointer(int level, const InternalKey& key) {
    compact_pointers_.push_back(std::make_pair(level, key));
  }

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  // 该函数只能在 VersionSet::LogAndApply 函数中调用，且该 Version 对象还没有被保存
  void AddFile(int level, uint64_t file, uint64_t file_size,
               const InternalKey& smallest, const InternalKey& largest) {
    FileMetaData f;
    f.number = file;
    f.file_size = file_size;
    f.smallest = smallest;
    f.largest = largest;
    new_files_.push_back(std::make_pair(level, f));
  }

  // Delete the specified "file" from the specified "level".
  void RemoveFile(int level, uint64_t file) {
    deleted_files_.insert(std::make_pair(level, file));
  }

  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string DebugString() const;

 private:
  friend class VersionSet;

  typedef std::set<std::pair<int, uint64_t>> DeletedFileSet;

  // 比较器名称
  std::string comparator_;
  // 当前日志文件的编号
  uint64_t log_number_;
  // 上一个日志文件的编号
  uint64_t prev_log_number_;
  // 下一个文件的编号
  uint64_t next_file_number_;
  // 最后一个序列号
  SequenceNumber last_sequence_;
  bool has_comparator_;
  bool has_log_number_;
  bool has_prev_log_number_;
  bool has_next_file_number_;
  bool has_last_sequence_;

  // 每个 level 对应的文件指针
  // 每个元素是一个 pair，表示某个 level 对应的文件指针
  std::vector<std::pair<int, InternalKey>> compact_pointers_;
  // 需要删除的文件
  // 每个元素是一个 pair，表示需要删除的文件的编号和 level
  DeletedFileSet deleted_files_;
  // 需要添加的文件
  // 每个元素是一个 pair，表示需要添加的文件的 level 和 FileMetaData 对象
  std::vector<std::pair<int, FileMetaData>> new_files_;
};

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_VERSION_EDIT_H_
