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

}; // namespace velevdb

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
