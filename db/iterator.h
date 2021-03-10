#ifndef VELEVDB_DB_ITERATOR_H_
#define VELEVDB_DB_ITERATOR_H_

#include <string>

namespace velevdb {

class Iterator {
 public:
  Iterator();

  Iterator(const Iterator&) = delete;
  Iterator& operator=(const Iterator&) = delete;

  virtual ~Iterator();

  virtual bool Valid() const = 0;

  virtual void SeekToFirst() = 0;

  virtual void SeekToLast() = 0;

  virtual void Seek(const std::string& target) = 0;

  virtual void Next() = 0;

  virtual void Prev() = 0;

  virtual std::string key() const = 0;

  virtual std::string value() const = 0;

 private:

};

}

#endif // VELEVDB_DB_ITERATOR_H_
