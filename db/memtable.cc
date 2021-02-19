#include "memtable.h"

namespace velevdb {

MemTable::MemTable(const InternalKeyComparator& comparator) : comparator_(comparator), table_(comparator) {}

MemTable::~MemTable() {}

void MemTable::Add(SequenceNumber seq, ValueType type, const string &key, const string &value) {
  return;
}

bool MemTable::Get(const InternalKey &key, string *value) {
  return false;
}
}
