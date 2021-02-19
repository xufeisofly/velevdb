#ifndef VELEVDB_DB_SKIPLIST_H_
#define VELEVDB_DB_SKIPLIST_H_

#include "dbformat.h"

namespace velevdb {

template <typename Key, class Comparator>
class SkipList {
 private:
  struct Node; // 声明一下，在后面定义public:

 public:
  explicit SkipList(Comparator cmp); // 使用 cmp 比较 skiplist 中的 node

  SkipList(const SkipList &) = delete;
  SkipList &operator=(const SkipList &) = delete;

  void Insert(const Key &key);
  bool Contains(const Key &key) const;

  class Iterator {
   public:
    explicit Iterator(const SkipList* sl);
    bool Valid() const;
    const Key& key() const;
    void Next();
    void Prev();
    void Seek(const Key& key);
    void SeekToFirst();
    void SeekToLast();

   private:
    const SkipList* sl_;
    Node* node_;
  };

 private:
  int const kMaxHeight = 12; // max height
  Node *const head_;         // 跳表的私有变量就是一个 head
  Comparator const comparator_;

  Node *NewNode(const Key &key,
                int height); // 因为 Node 有柔性数组，需要手动分配内存

  int RandomHeight();
  Node *FindGreaterOrEqual(const Key &key, Node **prev) const;
};

/*******************SkipList::Node*********************/

template <typename Key, class Comparator>
struct SkipList<Key, Comparator>::Node {
  explicit Node(const Key &k) : key(k) {}
  Key const key;

  Node *Next(int n) { return next_[n]; }

  void SetNext(int n, Node *node) { next_[n] = node; }

 private:
  Node *next_[1]; // 柔性数组，next_ 是一个 Node*
  // 的数组，不确定长度，这种定义方法只能放在最后一个 field
};

template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node *
    SkipList<Key, Comparator>::NewNode(const Key &key, int height) {
  // 由于柔性数组，所以需要手动分配内存，不然就会被截断？
  // TODO 不一定好使
  char* const mem = (char*)malloc(sizeof(Node) - 1 + sizeof(Node *) * height);
  return new (mem) Node(key);
}

/*******************SkipList::Iterator*********************/

template <typename Key, class Comparator>
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList* sl) {
  sl_ = sl;
  node_ = nullptr;
}

template <typename Key, class Comparator>
inline bool SkipList<Key, Comparator>::Iterator::Valid() const {
  return node_ != nullptr;
}

template <typename Key, class Comparator>
inline const Key& SkipList<Key, Comparator>::Iterator::key() const {
  assert(Valid());
  return node_->key;
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Next() {
  assert(Valid());
  node_ = node_->Next(0); // 0 height 的 Next Node 肯定是连续的
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Prev() {
  // TODO 先不实现，感觉没啥用
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key &key) {
  node_ = sl_->FindGreaterOrEqual(key, nullptr);
}

/*******************SkipList*********************/

template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(Comparator cmp)
    : comparator_(cmp), head_(NewNode(0, kMaxHeight)) {
  for (int i = 0; i < kMaxHeight; i++) {
    head_->SetNext(i, nullptr);
  }
}

template <typename Key, class Comparator>
inline int SkipList<Key, Comparator>::RandomHeight() {
  static unsigned int const kBranching = 4;
  int height = 1;
  while (height < kMaxHeight && rand() % kBranching == 0) {
    height++;
  }
  assert(height > 0);
  assert(height <= kMaxHeight);
  return height;
}

template <typename Key, class Comparator>
typename SkipList<Key, Comparator>::Node *
    SkipList<Key, Comparator>::FindGreaterOrEqual(const Key &key,
                                                  Node **prev) const {
  Node *c = head_;
  Node *nextAtlevel;

  for (int cl = kMaxHeight - 1; cl >= 0; cl--) {
    nextAtlevel = c->Next(cl);
    while (nextAtlevel != nullptr &&
           comparator_.Compare(nextAtlevel->key, key) < 0) {
      c = nextAtlevel;
      nextAtlevel = c->Next(cl);
    }
    if (prev != nullptr) {
      prev[cl] = c;
    }
  }

  return nextAtlevel;
}

template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key &key) const {
  Node* node = FindGreaterOrEqual(key, nullptr);
  if (node != nullptr && comparator_.Compare(node->key, key) == 0) {
    return true;
  }
  return false;
}

template <typename Key, class Comparator>
void SkipList<Key, Comparator>::Insert(const Key &key) {
  Node* prev[kMaxHeight];
  Node* node = FindGreaterOrEqual(key, prev);

  assert(node == nullptr || !(comparator_.Compare(node->key, key) == 0));

  int height = RandomHeight();
  node = NewNode(key, height);

  for (int i=0; i < height; i++) {
    node->SetNext(i, prev[i]->Next(i));
    prev[i]->SetNext(i, node);
  }
}

} // namespace velevdb

#endif // VELEVDB_DB_SKIPLIST_H_
