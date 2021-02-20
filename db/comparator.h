#ifndef VELEVDB_DB_COMPARATOR_H_
#define VELEVDB_DB_COMPARATOR_H_

#include <string>

namespace velevdb {

class Comparator {
 public:
  virtual ~Comparator();
  virtual int Compare(const std::string& a, const std::string& b) const = 0;
};

inline Comparator::~Comparator() = default;

}

#endif // VELEVDB_DB_COMPARATOR_H_
