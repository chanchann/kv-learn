#include "rocksdb/db.h"
#include <iostream>
#include <string>

int main() {
  // Open the database
  rocksdb::DB *db;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/db", &db);
  if (!status.ok()) {
    std::cerr << "Failed to open database: " << status.ToString() << std::endl;
    return 1;
  }

  // Delete a key
  rocksdb::WriteOptions write_options;
  status = db->Delete(write_options, "my_key");
  if (!status.ok()) {
    std::cerr << "Failed to delete key: " << status.ToString() << std::endl;
    return 1;
  }

  // Close the database
  delete db;

  return 0;
}
