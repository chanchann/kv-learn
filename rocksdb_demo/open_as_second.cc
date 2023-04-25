#include "rocksdb/db.h"
#include <iostream>
#include <vector>

using namespace rocksdb;

int main() {
  // Set up primary database instance
  DB *primary_db;
  Options primary_options;
  primary_options.create_if_missing = true;
  Status s = DB::Open(primary_options, "/tmp/primary_db", &primary_db);
  if (!s.ok()) {
    std::cerr << "Error opening primary database: " << s.ToString()
              << std::endl;
    return 1;
  }

  // Set up secondary database instance
  DB *secondary_db;
  Options secondary_options;
  secondary_options.create_if_missing = true;
  std::string secondary_path = "/tmp/secondary_db";
  std::string primary_name = "primary_db";
  std::vector<ColumnFamilyDescriptor> column_families;
  s = DB::OpenAsSecondary(secondary_options, primary_name, secondary_path,
                          column_families, nullptr, &secondary_db);
  if (!s.ok()) {
    std::cerr << "Error opening secondary database: " << s.ToString()
              << std::endl;
    return 1;
  }

  // Use primary database instance to write data
  WriteOptions write_options;
  write_options.sync = true;
  s = primary_db->Put(write_options, "key", "value");
  if (!s.ok()) {
    std::cerr << "Error writing to primary database: " << s.ToString()
              << std::endl;
    return 1;
  }

  // Use secondary database instance to read data
  ReadOptions read_options;
  std::string value;
  s = secondary_db->Get(read_options, "key", &value);
  if (!s.ok()) {
    std::cerr << "Error reading from secondary database: " << s.ToString()
              << std::endl;
    return 1;
  }
  std::cout << "Value: " << value << std::endl;

  // Clean up
  delete primary_db;
  delete secondary_db;

  return 0;
}
