#ifndef VELEVDB_DB_DBFORMAT_H_
#define VELEVDB_DB_DBFORMAT_H_

#include "comparator.h"

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
  return std::string(internal_key, internal_key.size()-8);
};

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
};


class InternalKeyComparator : public Comparator {
 public:
  int Compare(const InternalKey& a, const InternalKey& b) const;
  int Compare(const std::string& a, const std::string& b) const override;
};

inline int InternalKeyComparator::Compare(const InternalKey& a, const InternalKey& b) const {
  // 比较 internal key encode 成 std::string 后的值
  return Compare(a.Encode(), b.Encode());
}

} // namespace velevdb

#endif // VELEVDB_DB_DBFORMAT_H_
