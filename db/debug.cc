#include "dbformat.h"
#include "skiplist.h"
#include "memtable.h"

using namespace velevdb;

int main(int argc, char const *argv[]) {
  InternalKey a1("a", 1001, kTypeValue);
  InternalKey b1("b", 1001, kTypeValue);

  InternalKeyComparator *cmp;
  cmp->Compare2(a1, b1);
  return 0;
}
