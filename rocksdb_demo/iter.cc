#include <iostream>
#include <rocksdb/db.h>

int main() {
  rocksdb::DB *db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
  if (!status.ok()) {
    std::cerr << "Open database error: " << status.ToString() << std::endl;
    return -1;
  }

  rocksdb::WriteOptions write_options;
  rocksdb::ReadOptions read_options;

  // 写入数据
  db->Put(write_options, "key1", "value1");
  db->Put(write_options, "key2", "value2");
  db->Put(write_options, "key3", "value3");

  // 遍历数据
  rocksdb::Iterator *it = db->NewIterator(read_options);
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ": " << it->value().ToString()
              << std::endl;
  }
  if (!it->status().ok()) {
    std::cerr << "Iterator error: " << it->status().ToString() << std::endl;
  }
  delete it;

  delete db;
  return 0;
}
