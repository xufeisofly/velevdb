#ifndef VELEVDB_DB_MEMTABLETEST_H_
#define VELEVDB_DB_MEMTABLETEST_H_

#include <gtest/gtest.h>
#include "dbformat.h"
#include "memtable.h"

namespace velevdb {

TEST(MemTableTest, AddandGet) {
  InternalKeyComparator cmp;
  MemTable memtable(cmp);

  memtable.Add(1000, kTypeValue, "key1", "value1");
  std::string value;
  LookupKey lukey("key1", 1000);
  bool r = memtable.Get(lukey, &value);

  ASSERT_EQ(true, r);
  ASSERT_EQ("value1", value);
}

}

#endif // VELEVDB_DB_MEMTABLETEST_H_
