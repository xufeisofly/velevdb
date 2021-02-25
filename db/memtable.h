#ifndef VELEVDB_DB_MEMTABLE_H_
#define VELEVDB_DB_MEMTABLE_H_

#include "dbformat.h"
#include "skiplist.h"
#include "iterator.h"
#include <string>

using namespace std;

namespace velevdb {

class MemTable {
 public:
  explicit MemTable(const InternalKeyComparator& comparator);
  MemTable(const MemTable&) = delete;
  MemTable &operator=(const MemTable&) = delete;
  ~MemTable();

  // TODO Iterator
  Iterator* NewIterator();

  void Add(SequenceNumber seq, ValueType type, const string& key,
           const string& value);

  bool Get(const LookupKey& key, string* value);

 private:
  typedef SkipList<const char *, InternalKeyComparator> Table;

  Table table_;
  InternalKeyComparator comparator_;
};

} // namespace velevdb

#endif // VELEVDB_DB_MEMTABLE_H_
