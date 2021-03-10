#include <gtest/gtest.h>

#include "skiplist_test.h"
#include "dbformat_test.h"
#include "memtable_test.h"

namespace velevdb {

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

} // namespace velevdb
