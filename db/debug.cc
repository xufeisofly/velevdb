#include "dbformat.h"
#include "skiplist.h"
#include "memtable.h"

using namespace velevdb;

int main(int argc, char const *argv[]) {
  InternalKey a1("a", 1002, kTypeValue);
  InternalKey b1("a", 1001, kTypeValue);

  InternalKeyComparator cmp;
  int r = cmp.Compare(a1, b1);
  printf("%d ---", r);
  return 0;
}
