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
  ASSERT_EQ(1000 << 8 | kTypeValue, internal_key.seq_n_type());
}

}

#endif // VELEVDB_DB_DBFORMAT_TEST_H_
