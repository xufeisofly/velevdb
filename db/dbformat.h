#ifndef VELEVDB_DB_DBFORMAT_H_
#define VELEVDB_DB_DBFORMAT_H_

#include "comparator.h"
#include "coding.h"

namespace velevdb {

// SequenceNumber MVCC 的版本号
typedef uint64_t SequenceNumber;
// 定义最大的 SequenceNumber，不超过 56bit
static const SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);

// ValueType key action 的类型
enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };

// ExtractUserKey 从 internal_key string 中抽取 user_key
inline std::string ExtractUserKey(const std::string& internal_key) {
  assert(internal_key.size() >= 8);
  // 去掉低 8 位（即 SequenceNumber | ValueType 的部分）
  return std::string(internal_key, 0, internal_key.size()-8);
};

inline SequenceNumber ExtrackSequenceNumberWithType(const std::string& internal_key) {
  assert(internal_key.size() >= 8);
  const char* data = internal_key.c_str();
  size_t size = internal_key.size();
  return DecodeFixed64(data + size - 8);
}

int BytewiseCompare(const std::string &a, const std::string &b);


// internal key 需要包裹在这个类中，不要直接用 std::string，这样比较 key
// 的时候就不会直接使用 std::string 的比较器了
class InternalKey {
 private:
  std::string rep_;

 public:
  InternalKey() {}
  InternalKey(const std::string& user_key, SequenceNumber s, ValueType t);
  InternalKey(const InternalKey&) = delete;
  InternalKey& operator=(const InternalKey&) = delete;


  std::string Encode() const;
  std::string user_key() const;
  SequenceNumber seq_n_type() const;
};

class InternalKeyComparator : public Comparator {
 public:
  explicit InternalKeyComparator() = default;
  
  int Compare(const std::string &a, const std::string &b) const override;
  int Compare2(const InternalKey& a, const InternalKey& b) const;
};

// Compare 比较 InternalKey，先比较 user_key 部分，如果一样再比较 sequence
// number，谁大谁就小
inline int InternalKeyComparator::Compare2(const InternalKey &a,                                    const InternalKey &b) const {
  return Compare("a", "b");
}

class LookupKey {
 public:
  LookupKey(const std::string& user_key, SequenceNumber sequence);
  LookupKey(const LookupKey&) = delete;
  LookupKey& operator=(const LookupKey&) = delete;
  ~LookupKey();

  std::string memtable_key() const { return std::string(start_, end_ - start_); }
  std::string internal_key() const { return std::string(kstart_, end_ - kstart_); }
  std::string user_key() const { return std::string(kstart_, end_ - kstart_ - 8); }

 private:
  const char* start_;
  const char* kstart_;
  const char* end_;
};

inline LookupKey::~LookupKey() {
  delete [] start_;
}

} // namespace velevdb

#endif // VELEVDB_DB_DBFORMAT_H_
