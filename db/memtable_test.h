#ifndef VELEVDB_DB_MEMTABLETEST_H_
#define VELEVDB_DB_MEMTABLETEST_H_

#include <gtest/gtest.h>
#include "dbformat.h"
#include "memtable.h"

namespace velevdb {

TEST(MemTableTest, Add) {
  InternalKeyComparator cmp;
  MemTable memtable(cmp);

  memtable.Add(1000, kTypeValue, "key1", "value1");
}

}

#endif // VELEVDB_DB_MEMTABLETEST_H_
