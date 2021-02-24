#include "coding.h"

namespace velevdb {

// PutFixed64 将 uint64_t 填充到 8 个 bytes 中，返回 string
void PutFixed64(std::string *dst, uint64_t value) {
  char buf[sizeof(value)]; // sizeof(value) 肯定是 8 bytes
  EncodeFixed64(buf, value);
  dst->append(buf, sizeof(buf));
}

// 计算变长 int64 所需要的字节数（定长的不用算，不就是 64/8 嘛）
// 另外 uint64 的字节长度肯定是小于等于 8 的，所以 return int
int VarintLength(uint64_t v) {
  int len = 1;
  while(v >= 128) { // 为什么不是 258 和 >>= 8？不懂了
    v >>= 7;
    len++;
  }
  return len;
}

char* EncodeVarint32(char* dst, uint32_t v) {
  uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
  static const int B = 128;
  if (v < (1 << 7)) {
    *(ptr++) = v;
  } else if (v < (1 << 14)) {
    *(ptr++) = v | B;
    *(ptr++) = v >> 7;
  } else if (v < (1 << 21)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = v >> 14;
  } else if (v < (1 << 28)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = v >> 21;
  } else {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = (v >> 21) | B;
    *(ptr++) = v >> 28;
  }
  return reinterpret_cast<char*>(ptr);
}

} // namespace velevdb
