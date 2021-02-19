#ifndef VELEVDB_DB_DBFORMAT_H_
#define VELEVDB_DB_DBFORMAT_H_

#include "comparator.h"

namespace velevdb {
// SequenceNumber MVCC 的版本号
typedef uint64_t SequenceNumber;
// ValueType key action 的类型
enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };

// internal key 需要包裹在这个类中，不要直接用 std::string，这样比较 key
// 的时候就不会直接使用 std::string 的比较器了
class InternalKey {
 private:
  std::string rep_;

 public:
  InternalKey() {}
  InternalKey(const std::string& user_key, SequenceNumber s, ValueType t) {
  };
  std::string Encode() const { return rep_; }
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
