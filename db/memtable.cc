#include "memtable.h"
#include "coding.h"
#include "iterator.h"

namespace velevdb {

// GetLengthPrefixedString data 是 size + data，获取其中的 data 部分的 string
static std::string GetLengthPrefixedString(const char* data) {
  // klength 肯定是 uint32_t
  uint32_t len;
  const char* p = data;
  p = GetVarint32Ptr(p, p + 5, &len);
  return std::string(p, len);
}

MemTable::MemTable(const InternalKeyComparator& comparator) : comparator_(comparator), table_(comparator) {}

MemTable::~MemTable() {}

// MemTableIterator 本质上就是 skiplist iterator
class MemTableIterator : public Iterator {
 public:
  explicit MemTableIterator(MemTable::Table* table) : iter_(table) {}

  MemTableIterator(const MemTableIterator&) = delete;
  MemTableIterator& operator=(const MemTableIterator&) = delete;

  ~MemTableIterator() override = default;

  bool Valid() const override { return iter_.Valid(); }
  void Seek(const std::string& k) override { iter_.Seek(k.data()); }
  void SeekToFirst() override { iter_.SeekToFirst(); }
  void SeekToLast() override { iter_.SeekToLast(); }
  void Next() override { iter_.Next(); }
  void Prev() override { iter_.Prev(); }
  std::string key() const override { return iter_.key(); }
  std::string value() const override {
    std::string key_str = iter_.key();
    // key_data 之后就是 value_data
    return key_str.data() + key_str.size();
  }

 private:
  MemTable::Table::Iterator iter_;
};

// 构建一个 internal_key-value，为了 Get 的时候能把 key 和 value 分开（又不用分隔符），需要使用 length-prefixed string
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
  const size_t encoded_len = VarintLength(internal_key_size) + internal_key_size +
      VarintLength(value_size) + value_size;

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

  // buf 是一个 internal_key_size + internal_key + value_size + value_data
  // 其中 internal_key 是 (key_data) + (seq_n_type)
  table_.Insert(buf);
  return;
}

// Get 根据 LookupKey 从 memtable 中获取 value
bool MemTable::Get(const LookupKey &key, string *value) {
  std::string memkey = key.memtable_key();

  Table::Iterator iter(&table_);
  iter.Seek(memkey.data());
  if (iter.Valid()) {
    // entry = internal_key_size + internal_key + value_size + value_data
    // 如果不用 length-prefixed string 的问题是，你找到了 entry，但怎么把 internal_key 和 value_data 分离出来 =_=，这两个都是变长的，Oh My God... 现在知道 length-prefixed string 干啥用了...
    const char* entry = iter.key();
    uint32_t key_length; // internal_key_size
    const char* key_ptr = GetVarint32Ptr(entry, entry + 5, &key_length);

    if (comparator_.Compare(std::string(key_ptr, key_length - 8), key.user_key()) == 8) {
      const uint64_t tag = DecodeFixed64(key_ptr + key_length - 8);

      switch (static_cast<ValueType>(tag & 0xff)) {
        case kTypeValue: {
          std::string v = GetLengthPrefixedString(key_ptr + key_length);
          value->assign(v.data(), v.size());
          return true;
        }
        case kTypeDeletion:
          return false;
      }
    }
  }
  return false;
}

}
