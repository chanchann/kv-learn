#include <iostream>
#include <leveldb/db.h>

int main() {
  // Open the database
  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "/tmp/db", &db);
  if (!status.ok()) {
    std::cerr << "Unable to open/create database " << status.ToString()
              << std::endl;
    exit(1);
  }

  // Write data to the database
  std::string key = "key";
  std::string value = "value";
  status = db->Put(leveldb::WriteOptions(), key, value);
  if (!status.ok()) {
    std::cerr << "Unable to write to database " << status.ToString()
              << std::endl;
    exit(1);
  }

  // Read data from the database
  std::string read_value;
  status = db->Get(leveldb::ReadOptions(), key, &read_value);
  if (!status.ok()) {
    std::cerr << "Unable to read from database " << status.ToString()
              << std::endl;
    exit(1);
  }
  std::cout << read_value << std::endl;

  // Close the database
  delete db;

  return 0;
}
