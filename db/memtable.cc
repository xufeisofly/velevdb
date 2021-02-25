#include "memtable.h"
#include "coding.h"
#include "iterator.h"

namespace velevdb {

MemTable::MemTable(const InternalKeyComparator& comparator) : comparator_(comparator), table_(comparator) {}

MemTable::~MemTable() {}

class MemTableIterator : public Iterator {
 public:
};

void MemTable::Add(SequenceNumber seq, ValueType type, const string &key, const string &value) {
  // format an entry:
  // key_size
  // key bytes
  // value_size
  // value bytes
  size_t key_size = key.size();
  size_t internal_key_size = key_size + 8; // 8 是 seq 的长度
  size_t value_size = value.size();

  // 首先计算 entry 需要的字节数，分配内存
  const size_t encoded_len = VarintLength(internal_key_size) +
      internal_key_size + VarintLength(value_size) + value_size;
  // 分配内存
  char* buf;
  // buf 中放 internal_key_size
  char* p = EncodeVarint32(buf, internal_key_size);
  // buf 中放 key data
  std::memcpy(p, key.data(), key_size);
  p += key_size;
  // buf 中放 seq_n_type
  EncodeFixed64(p, (seq << 8) | type);
  p += 8;
  // buf 中放 value_size
  p = EncodeVarint32(p, value_size);
  // buf 中放 value data
  std::memcpy(p, value.data(), value_size);
  p += value_size;

  assert(p == buf + encoded_len);

  table_.Insert(buf);
  return;
}

bool MemTable::Get(const LookupKey &key, string *value) {
  std::string memkey = key.memtable_key();
  
  return false;
}
}
