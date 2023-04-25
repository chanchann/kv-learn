## snippet

### Open

```cpp
#include <cassert>
#include "rocksdb/db.h"

rocksdb::DB* db;
rocksdb::Options options;
options.create_if_missing = true;
rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
assert(status.ok());
...
```


### ConvertOptions

```cpp
#include "rocksdb/utilities/leveldb_options.h"

rocksdb::LevelDBOptions leveldb_options;
leveldb_options.option1 = value1;
leveldb_options.option2 = value2;
...
rocksdb::Options options = rocksdb::ConvertOptions(leveldb_options);
```

### Options

```cpp
rocksdb::Status s;
s = db->SetOptions(write_buffer_size);
assert(s.ok());
s = db->SetDBOptions(max_background_flushes);
assert(s.ok());
```

key :

RocksDB配置文件

动态修改

### Status

```cpp
rocksdb::Status s = ...;
if (!s.ok()) cerr << s.ToString() << endl;
```

### close

```cpp
delete db;

// or
// recommend
Status s = db->Close();
delete db;
```

### Read

```cpp
std::string value;
rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key1, &value);
if (s.ok()) s = db->Put(rocksdb::WriteOptions(), key2, value);
if (s.ok()) s = db->Delete(rocksdb::WriteOptions(), key1);
```

```cpp
MultiGet

性能分析
```

### Write

1. 原子更新

2. 加速

```cpp
#include "rocksdb/write_batch.h"
...
std::string value;
rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key1, &value);
if (s.ok()) {
  rocksdb::WriteBatch batch;
  batch.Delete(key1);
  batch.Put(key2, value);
  s = db->Write(rocksdb::WriteOptions(), &batch);
}
```


