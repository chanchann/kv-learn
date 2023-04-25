#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

int main() {
  rocksdb::DB *db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
  assert(status.ok());

  rocksdb::WriteBatch batch;
  batch.Put("key1", "value1");
  batch.Put("key2", "value2");
  batch.Delete("key3");

  status = db->Write(rocksdb::WriteOptions(), &batch);
  assert(status.ok());

  delete db;
  return 0;
}
