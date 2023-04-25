#include <iostream>
#include <rocksdb/db.h>
#include <rocksdb/utilities/options_util.h>

int main() {
  // create a RocksDB database instance
  /*
  rocksdb::DB *db;
  rocksdb::ConfigOptions config_options;
  config_options.ignore_unknown_options = false;
  config_options.input_strings_escaped = true;
  rocksdb::Options options;
  // load options from file
  auto status = rocksdb::LoadOptionsFromFile(
      "./rocksdb_option_file_example.ini", &options);

  // check if options were loaded successfully
  if (!status.ok()) {
    std::cerr << "Failed to load options: " << status.ToString() << std::endl;
    return 1;
  } else {
    std::cout << "Options loaded successfully!" << std::endl;
  }

  status = rocksdb::DB::Open(options, "/tmp/db", &db);

  if (!status.ok()) {
    std::cerr << "Failed to open database: " << status.ToString() << std::endl;
    return 1;
  }

  delete db;
  */
  return 0;
}
