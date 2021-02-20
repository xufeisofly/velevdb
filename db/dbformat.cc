#include "dbformat.h"
#include "coding.h"

namespace velevdb {

/******** InternalKey *******/

// PackSequenceAndType 组合 SequenceNumber 和 ValueType 信息，只占用 64 bit
uint64_t PackSequenceAndType(uint64_t seq, ValueType t) {
  assert(seq <= kMaxSequenceNumber);
  // 用 64 bit 同时携带了 SequenceNumber 和 ValueType 信息
  return (seq << 8 | t);
}

InternalKey::InternalKey(const std::string &user_key, SequenceNumber s,
                         ValueType t) {
  rep_ = user_key;
  PutFixed64(&rep_, PackSequenceAndType(s, t));
}

std::string InternalKey::Encode() const {
  return rep_;
}

std::string InternalKey::user_key() const {
  return ExtractUserKey(rep_);
}

/******** InternalKeyComparator *******/

int InternalKeyComparator::Compare(const std::string& a, const std::string& b) const {
  return 0;
}

}



