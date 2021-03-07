#ifndef VELEVDB_DB_DBFORMAT_TEST_H_
#define VELEVDB_DB_DBFORMAT_TEST_H_

#include <gtest/gtest.h>
#include "dbformat.h"

namespace velevdb {

TEST(InternalKeyTest, user_key) {
  std::string user_key = "key";
  SequenceNumber s = 1000;
  ValueType t = kTypeValue;
  InternalKey internal_key(user_key, s, t);

  ASSERT_EQ("key", internal_key.user_key());
  ASSERT_EQ(s << 8 | kTypeValue, internal_key.seq_n_type());
}

TEST(InternalKeyComparatorTest, Compare) {
  InternalKey a1("a", 1001, kTypeValue);
  InternalKey b1("b", 1001, kTypeValue);
  InternalKey a2("a", 1002, kTypeValue);
  InternalKey b2("b", 1001, kTypeValue);

  InternalKeyComparator* cmp;
  ASSERT_EQ(-1, cmp->Compare2(a1, b1));
  // ASSERT_EQ(-1, cmp->Compare(a1, a2));
  // ASSERT_EQ(0, cmp->Compare(b1, b2));
}

}

#endif // VELEVDB_DB_DBFORMAT_TEST_H_
