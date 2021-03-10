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
  memtable.Add(1001, kTypeValue, "key1", "value2");
  std::string value;
  LookupKey lukey("key1", 1000);
  bool r = memtable.Get(lukey, &value);

  ASSERT_EQ(true, r);
  ASSERT_EQ("value1", value);

  LookupKey lukey2("key1", 1001);
  bool r2 = memtable.Get(lukey2, &value);
  ASSERT_EQ(true, r2);
  ASSERT_EQ("value2", value);
}

}

#endif // VELEVDB_DB_MEMTABLETEST_H_
