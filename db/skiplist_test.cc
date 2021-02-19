#include <gtest/gtest.h>
#include "skiplist.h"

namespace velevdb {

class IntComparator : public Comparator {
 public:
  int Compare(const int& a, const int& b) const;
  int Compare(const std::string& a, const std::string& b) const {return 0;};
};

int IntComparator::Compare(const int &a, const int &b) const {
  if (a > b) {
    return 1;
  } else if (a < b) {
    return -1;
  }
  return 0;
}

typedef int Key;

TEST(SkipTest, Empty) {
  IntComparator cmp;
  SkipList<Key, IntComparator> sl(cmp);
  ASSERT_TRUE(!sl.Contains(10));
}

TEST(SkipTest, InsertAndContain) {
  int N = 100;
  IntComparator cmp;
  SkipList<Key, IntComparator> sl(cmp);
  for (int i = 0; i < N; i++) {
    sl.Insert(i);
  }

  for (int i = 0; i < N; i++) {
    ASSERT_TRUE(sl.Contains(i));
  }
}

TEST(SkipTest, InsertAndLookup) {
  int N = 100;
  IntComparator cmp;
  SkipList<Key, IntComparator> sl(cmp);
  for (int i = 0; i < N; i++) {
    sl.Insert(i);
  }

  SkipList<Key, IntComparator>::Iterator iter(&sl);
  ASSERT_TRUE(!iter.Valid());

  for (int i = 0; i < N; i++) {
    iter.Seek(i);
    ASSERT_TRUE(iter.Valid());
    ASSERT_EQ(i, iter.key());
  }
  iter.Seek(N+1);
  ASSERT_TRUE(!iter.Valid());
}

}; // namespace velevdb

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
