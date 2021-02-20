#include "coding.h"

namespace velevdb {

// PutFixed64 将 uint64_t 填充到 8 个 bytes 中，返回 string
void PutFixed64(std::string *dst, uint64_t value) {
  char buf[sizeof(value)]; // sizeof(value) 肯定是 8 bytes
  EncodeFixed64(buf, value);
  dst->append(buf, sizeof(buf));
}

}
