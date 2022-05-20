//
// Created by diffraction on 22/04/2022.
//

#include <iostream>
#include <cstddef>
#include <exception>
#include <memory>
#include <iterator>
#include <type_traits>


template <class T, size_t N>
class StackAllocator;


template <size_t N>
class StackStorage {
private:
  char memory_[N];
public:

  template <class T, size_t M>
  friend
  class StackAllocator;

  StackStorage() {
    memory_[0] = 'e';
  }

  StackStorage(StackStorage<N> &other) : StackStorage() {};

  ~StackStorage() = default;

  template <size_t M>
  bool operator==(const StackStorage<M> &other) {
    return &memory_[0] == &other.memory_[0];
  }

  template <size_t M>
  bool operator!=(const StackStorage<M> &other) {
    return !(*this == other);
  }
};

#define al_T alignof(T)
#define sz_T sizeof(T)
#define al_node alignof(Node)
#define sz_node sizeof(Node)

struct Node {
    size_t index_;
    size_t size_;
    Node *next_;
    Node *prev_;

    Node(int index = 0, int size = 0, Node *next = nullptr, Node *prev = nullptr) : index_(index), size_(size),
                                                                                    next_(next), prev_(prev) {};

    template <class V, size_t M>
    friend
    class StackAllocator;
};

template <class T, size_t N>
class StackAllocator {
private:


  char *pool_;
  int *number_of_node_;
  Node *first_node_;
public:

  friend struct Node;

  template <class V, size_t M>
  friend
  class StackAllocator;

  StackAllocator(StackStorage<N> &other);

  template <class U>
  StackAllocator(const StackAllocator<U, N> &other);

  template <typename U>
  StackAllocator &operator=(const StackAllocator<U, N> &other) = delete;

  ~StackAllocator();

  T *allocate(size_t number);

  void deallocate(T *point, size_t number_of_elem);

  using value_type = T;

  StackAllocator<T, N> &select_on_container_copy_construction() {
    return StackAllocator<T, N>(*this);
  };

  template <class U, size_t M>
  bool operator==(const StackAllocator<U, M> &other) {
    return pool_ == other.pool_;
  }

  template <class U, size_t M>
  bool operator!=(const StackAllocator<U, M> &other) {
    return pool_ != other.pool_;
  }

  template <class U>
  struct rebind {
      typedef StackAllocator<U, N> other;
  };

private:
  // to find the best node
  bool is_suitable(size_t number, Node *node);

  bool is_more_suitable(Node *node1, Node *node2);

  bool is_precise(Node *node);

  bool is_precise_left(Node *node);

  bool is_precise_right(Node *node);

  // to get memory from node

  T *get_memory(Node *node, size_t number_of_elem);

  T *place_to_right(Node *node, size_t number_of_elem);

  T *place_to_left(Node *node, size_t number_of_elem);

  // find node with input index or first that left/right to index

  Node *find_node_to_left(size_t index);

  Node *find_node_to_right(size_t index);

  // carrying of memory for node

  void delete_empty_node(Node *node);

  void create_node(int index, int size, Node *left = nullptr, Node *right = nullptr);
};

template <typename T, size_t N>
StackAllocator<T, N>::StackAllocator(StackStorage<N> &pool): pool_(&pool.memory_[0]),
                                                             number_of_node_(
                                                                     nullptr), first_node_(nullptr) {
  char *cur = pool_ + 1;
  size_t dif_1 = (alignof(int) - reinterpret_cast<size_t>(cur) % alignof(int)) % alignof(int);
  size_t dif_2 = (al_node - reinterpret_cast<size_t>(cur + sizeof(int)) % al_node) % al_node;

  if (pool_[0] != 'e') { // in this case another allocator already has been living on this storage
    number_of_node_ = reinterpret_cast<int *>(cur);
    cur += sizeof(int);
    cur += dif_2;
    first_node_ = reinterpret_cast<Node *>(cur);
    return;
  }

  // number_of_node_

  cur += dif_1;
  new(cur) int(1);
  number_of_node_ = reinterpret_cast<int *>(cur);
  cur += sizeof(int);

  // first_node_
  cur += dif_2;
  new(cur) Node(cur - pool_ + sz_node, N - (cur - pool_) - sz_node, reinterpret_cast<Node *>(cur),
                reinterpret_cast<Node *>(cur));
  first_node_ = reinterpret_cast<Node *>(cur);

  // for next allocators on this storage
  pool_[0] = 'n';
}

template <typename T, size_t N>
template <typename U>
StackAllocator<T, N>::StackAllocator(const StackAllocator<U, N> &other): pool_(other.pool_),
                                                                         number_of_node_(other.number_of_node_),
                                                                         first_node_(other.first_node_) {}


template <typename T, size_t N>
StackAllocator<T, N>::~StackAllocator() = default;

template <typename T, size_t N>
T *StackAllocator<T, N>::allocate(size_t number) {
  Node *ans = nullptr;
  Node *cur = first_node_->next_;
  while (cur != first_node_) {
    if (is_suitable(number, cur)) {
      ans = cur;
      break;
    }
    cur = cur->next_;
  }
  if (is_suitable(number, first_node_)) {
    ans = (is_more_suitable(ans, first_node_) ? ans : first_node_);
  }
  if (ans == nullptr) {
    throw std::bad_alloc();
  }
  return get_memory(ans, number);
}

template <typename T, size_t N>
void StackAllocator<T, N>::deallocate(T *point, size_t number_of_elem) {
  size_t left_index = reinterpret_cast<char *>(point) - pool_; // begin of memory
  size_t right_index = left_index + number_of_elem * sz_T; // end of memory
  Node *left = find_node_to_left(left_index - 1);
  if (left == nullptr) {
    left = first_node_;
  }
  Node *right = left->next_;
  bool left_edge = (left->index_ + left->size_ == left_index);
  bool right_edge = (right->index_ == right_index);
  if (left_edge && right_edge) {
    left->size_ += number_of_elem * sz_T + right->size_;
    right->size_ = 0;
    delete_empty_node(right);
    return;
  }
  if (left_edge) {
    left->size_ += number_of_elem * sz_T;
    return;
  }
  if (right_edge) {
    right->index_ = left_index;
    right->size_ += number_of_elem * sz_T;
    return;
  }
  create_node(left_index, number_of_elem * sz_T, left, right);
}

template <typename T, size_t N>
bool StackAllocator<T, N>::is_suitable(size_t number, Node *node) {
  size_t dif = (al_T - reinterpret_cast<size_t>(pool_ + node->index_) % al_T) % al_T;
  return (node->size_ - dif) >= (number * sz_T);
}

template <typename T, size_t N>
bool StackAllocator<T, N>::is_more_suitable(Node *node1, Node *node2) {
  if (node1 == nullptr) {
    return false;
  }
  if (node2 == nullptr) {
    return true;
  }
  bool last_ans = node1->size_ < node2->size_;
  if (node1 == first_node_) {
    return false;
  }
  if (node2 == first_node_) {
    return true;
  }
  if (is_precise(node1)) {
    if (is_precise(node2)) {
      return last_ans;
    }
    return true;
  }
  if (is_precise(node2)) {
    return false;
  }
  return last_ans;
}

template <typename T, size_t N>
bool StackAllocator<T, N>::is_precise(Node *node) {
  return is_precise_right(node) || is_precise_left(node);
}

template <typename T, size_t N>
bool StackAllocator<T, N>::is_precise_left(Node *node) {
  return (reinterpret_cast<size_t>(node->index_ + pool_) % al_T) == 0;
}

template <typename T, size_t N>
bool StackAllocator<T, N>::is_precise_right(Node *node) {
  return reinterpret_cast<size_t>(node->index_ + pool_ + node->size_ - sz_T) % al_T == 0;
}

template <typename T, size_t N>
T *StackAllocator<T, N>::get_memory(Node *node, size_t number_of_elem) {
  // perfect placing
  if (is_precise_left(node) && node != first_node_) {
    return place_to_left(node, number_of_elem);
  }
  if (is_precise_right(node)) {
    return place_to_right(node, number_of_elem);
  }
  // in case we can't create new node for memory block
  if ((node == first_node_ && node->size_ < (sz_node + number_of_elem * sz_T)) || first_node_->size_ < sz_node) {
    throw std::bad_alloc();
  }
  // cut node
  size_t dif = reinterpret_cast<size_t>(node->index_ + pool_ + node->size_ - sz_T) % al_T;
  size_t index = node->index_ + node->size_ - dif;
  node->size_ -= dif;
  create_node(index, dif, node);
  return place_to_right(node, number_of_elem);
}

template <typename T, size_t N>
T *StackAllocator<T, N>::place_to_right(Node *node, size_t number_of_elem) {
  size_t index = node->index_ + node->size_ - number_of_elem * sz_T;
  T *point = reinterpret_cast<T *>(pool_ + index);
  node->size_ = (index - node->index_);
  if (node->size_ == 0 && node != first_node_) {
    delete_empty_node(node);
  }
  return point;
}

template <typename T, size_t N>
T *StackAllocator<T, N>::place_to_left(Node *node, size_t number_of_elem) {
  T *point = reinterpret_cast<T *>(pool_ + node->index_);
  node->size_ -= (sz_T * number_of_elem);
  if (node->size_ == 0 && node != first_node_) { // node is perfect simple memory
    delete_empty_node(node);
  } else if (node->size_ != 0 && node == first_node_) { // first node cant be deleted
    create_node(first_node_->index_ + sz_T * number_of_elem, node->size_, first_node_);
  } else { // node is simple - just cutting
    node->index_ += (sz_T * number_of_elem);
  }
  return point;
}

template <typename T, size_t N>
Node *StackAllocator<T, N>::find_node_to_left(size_t index) {
  Node *cur = first_node_->prev_;
  while (cur->index_ > index) {
    cur = cur->prev_;
  }
  if (cur == first_node_ && first_node_->size_ == 0) {
    return nullptr;
  }
  return cur;
}

template <typename T, size_t N>
Node *StackAllocator<T, N>::find_node_to_right(size_t index) {
  if (first_node_->index_ + first_node_->size_ > index) {
    return first_node_;
  }
  Node *cur = first_node_->next_;
  while (cur->index_ + cur->size_ <= index && cur != first_node_) {
    cur = cur->next_;
  }
  if (cur == first_node_) {
    return nullptr;
  }
  return cur;
}

template <typename T, size_t N>
void StackAllocator<T, N>::delete_empty_node(Node *node) {
  node->prev_->next_ = node->next_;
  node->next_->prev_ = node->prev_;
  --*number_of_node_;
  first_node_->index_ -= sz_node;
  first_node_->size_ += sz_node;
  if (first_node_ + (*number_of_node_) == node) {
    return;
  }
  Node *last_node = first_node_ + *number_of_node_;
  node->size_ = last_node->size_;
  node->index_ = last_node->index_;
  last_node->prev_->next_ = node;
  last_node->next_->prev_ = node;
  node->next_ = last_node->next_;
  node->prev_ = last_node->prev_;
}

template <typename T, size_t N>
void StackAllocator<T, N>::create_node(int index, int size, Node *left, Node *right) {
  if (left == nullptr && right == nullptr) {
    left = find_node_to_left(index - 1);
    if (left == nullptr) {
      left = first_node_;
    }
    right = first_node_->next_;
  }
  if (left == nullptr) {
    left = right->prev_;
  }
  if (right == nullptr) {
    right = left->next_;
  }
  Node *new_node = first_node_ + *(number_of_node_);
  ++*number_of_node_;
  first_node_->size_ -= sz_node;
  first_node_->index_ += sz_node;
  new_node->index_ = index;
  new_node->size_ = size;
  new_node->next_ = right;
  new_node->prev_ = left;
  left->next_ = new_node;
  right->prev_ = new_node;
}
