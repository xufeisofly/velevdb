#include "dbformat.h"
#include "coding.h"
#include <iostream>


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

SequenceNumber InternalKey::seq_n_type() const {
  return ExtrackSequenceNumberWithType(rep_);
}

/******** InternalKeyComparator *******/

// BytewiseCompare 比较 string 大小，这样就不用再单独搞个 BytewiseComparator 了
int BytewiseCompare(const std::string& a, const std::string& b) {
  size_t a_size = a.size();
  size_t b_size = b.size();
  const size_t min_len = (a_size < b_size) ? a_size : b_size;
  int r = memcmp(a.data(), b.data(), min_len);
  if (r == 0) {
    if (a_size < b_size)
      r = -1;
    else if (a_size > b_size)
      r = +1;
  }
  return r;
}

// Compare MemTable 实际调用的是它，因为构造时 char* 会自动转成 string
int InternalKeyComparator::Compare(const std::string &a, const std::string &b) const {
  int r = BytewiseCompare(ExtractUserKey(a), ExtractUserKey(b));
  if (r == 0) {
    const uint64_t aseq = ExtrackSequenceNumberWithType(a);
    const uint64_t bseq = ExtrackSequenceNumberWithType(b);
    if (aseq > bseq)
      r = -1;
    else if (aseq < bseq)
      r = +1;
  }
  return r;
}

LookupKey::LookupKey(const std::string& user_key, SequenceNumber s) {
  size_t usize = user_key.size();
  char* dst = new char[usize + 8]; // TODO 不用手动分配内存吗，当成变长数组使用？

  start_ = dst;
  dst = EncodeVarint32(dst, usize + 8);
  kstart_ = dst;
  std::memcpy(dst, user_key.data(), usize);
  dst += usize;
  EncodeFixed64(dst, PackSequenceAndType(s, kTypeValue));
  dst += 8;
  end_ = dst;
}

}



