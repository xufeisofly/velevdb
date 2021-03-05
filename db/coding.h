#ifndef VELEVDB_DB_CODING_H_
#define VELEVDB_DB_CODING_H_

#include <sys/types.h>
#include <string>

namespace velevdb {

void PutFixed64(std::string* dst, uint64_t value);
int VarintLength(uint64_t v);
char *EncodeVarint32(char *dst, uint32_t v);

const char* GetVarint32PtrFallback(const char* p, const char* limit, uint32_t* value);

// EncodeFixed64 将 uint64_t 写入到 8 个 byte 中
inline void EncodeFixed64(char *dst, uint64_t value) {
  uint8_t* buffer = reinterpret_cast<uint8_t*>(dst);

  buffer[0] = static_cast<uint8_t>(value);
  buffer[1] = static_cast<uint8_t>(value >> 8);
  buffer[2] = static_cast<uint8_t>(value >> 16);
  buffer[3] = static_cast<uint8_t>(value >> 24);
  buffer[4] = static_cast<uint8_t>(value >> 32);
  buffer[5] = static_cast<uint8_t>(value >> 40);
  buffer[6] = static_cast<uint8_t>(value >> 48);
  buffer[7] = static_cast<uint8_t>(value >> 56);
};

// DecodeFixed64 把 char* 转换成 uint64_t 类型
// 原理：先转成 uint8*，然后再放大成 uint64
inline uint64_t DecodeFixed64(const char* ptr) {
  const uint8_t* const buffer = reinterpret_cast<const uint8_t*>(ptr);

  return (static_cast<uint64_t>(buffer[0])) |
         (static_cast<uint64_t>(buffer[1]) << 8) |
         (static_cast<uint64_t>(buffer[2]) << 16) |
         (static_cast<uint64_t>(buffer[3]) << 24) |
         (static_cast<uint64_t>(buffer[4]) << 32) |
         (static_cast<uint64_t>(buffer[5]) << 40) |
         (static_cast<uint64_t>(buffer[6]) << 48) |
         (static_cast<uint64_t>(buffer[7]) << 56);
}

// GetVarint32Ptr: char* p 是一个 varint32(k_len) + k_data，此方法从中摘出
// varint32 的意思是最大是 4 个字节，最小是 1 个字节，长度不定
// k_data，返回 k_data 的起始指针
// 4 个字节，能存的长度为 2^28(每个字节的第 8 位不用不知道为什么)
// limit 是 p 最远的范围
inline const char* GetVarint32Ptr(const char* p, const char* limit, uint32_t* value) {
  if (p < limit) {
    // 取长度的最低 1 字节内容，确实是不是只有这一个字节
    uint32_t result = *(reinterpret_cast<const uint8_t*>(p));
    // 如果 klength 的第 8 bit == 0，说明长度只占了 1 位
    if ((result & 128) == 0) {
      *value = result;
      return p + 1;
    }
  }
  return GetVarint32PtrFallback(p, limit, value);
}
}

#endif // VELEVDB_DB_CODING_H_
