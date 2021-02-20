#ifndef VELEVDB_DB_CODING_H_
#define VELEVDB_DB_CODING_H_

#include <sys/types.h>
#include <string>

namespace velevdb {

void PutFixed64(std::string* dst, uint64_t value);

// EncodeFixed64 将 uint64_t 写入到 8 个 byte 中
inline void EncodeFixed64(char* dst, uint64_t value) {
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

}

#endif // VELEVDB_DB_CODING_H_
