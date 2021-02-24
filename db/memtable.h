#ifndef VELEVDB_DB_MEMTABLE_H_
#define VELEVDB_DB_MEMTABLE_H_

#include "dbformat.h"
#include "skiplist.h"
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

  void Add(SequenceNumber seq, ValueType type, const string& key,
           const string& value);

  bool Get(const InternalKey& key, string* value);

 private:
  SkipList<const char*, InternalKeyComparator> table_;
  InternalKeyComparator comparator_;
};
} // namespace velevdb

#endif // VELEVDB_DB_MEMTABLE_H_
