//
/// Created by diffraction on 01/05/2022.
//




#include <iostream>
#include <cstddef>
#include <exception>
#include <memory>
#include <iterator>
#include <type_traits>
#include <functional>

#ifndef UNTITLED_UNORDERED_MAP_H
#define UNTITLED_UNORDERED_MAP_H

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
class UnorderedMap;


template <typename T, class allocator = std::allocator<T>>
class List {
private:
  struct Node;
public:
  template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
  friend
  class UnorderedMap;

  using allocator_type = typename allocator::template rebind<typename List<T, allocator>::Node>::other;
  using value_type = T;
private:


  struct BaseNode {
      Node *next_;
      Node *prev_;

      BaseNode() : next_(nullptr), prev_(nullptr) {
        next_ = reinterpret_cast<Node *>(this);
        prev_ = reinterpret_cast<Node *>(this);
      };

      BaseNode &operator=(const BaseNode &other) = default;

      ~BaseNode() = default;
  };

  struct Node : BaseNode {
      using BaseNode::next_;
      using BaseNode::prev_;
      T value_;

      template <class ...Args>
      Node(Args &&... args) : value_(std::forward<Args>(args)...) {
        prev_ = nullptr;
        next_ = nullptr;
      }

      Node() : value_() {
        prev_ = nullptr;
        next_ = nullptr;
      }

      ~Node() = default;
  };

  struct const_iterator : protected std::forward_iterator_tag {
  protected:
    Node *node_;
  public:

    friend struct List::Node;
    friend struct List::BaseNode;

    template <class U, class allocator_other>
    friend
    class List;

    typedef const T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    const_iterator(Node *node) : node_(node) {};

    const_iterator &operator=(const const_iterator &other) = default;

    const_iterator(const const_iterator &other) = default;

    const_iterator &operator++();

    const_iterator operator++(int);

    const_iterator &operator--();

    const_iterator operator--(int);

    const T &operator*();

    const T *operator->();

    bool operator==(const const_iterator &other);

    bool operator!=(const const_iterator &other);
  };

  struct iterator : public const_iterator {
  private:
    using const_iterator::node_;
  public:

    template <class U, class allocator_other>
    friend
    class List;

    friend struct List::Node;
    friend struct List::BaseNode;

    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    iterator(Node *node) : const_iterator(node) {};

    iterator(const iterator &other) = default;

    iterator &operator++();

    iterator operator++(int);

    iterator &operator--();

    iterator operator--(int);

    iterator &operator=(const iterator &other) = default;

    T &operator*();

    T *operator->();
  };

public:
  using const_iterator = List<T, allocator>::const_iterator;
  using iterator = List<T, allocator>::iterator;
  using reverse_iterator = std::reverse_iterator<typename List<T, allocator>::iterator>;
  using const_reverse_iterator = std::reverse_iterator<typename List<T, allocator>::const_iterator>;
  typedef std::ptrdiff_t difference_type;

private:
  BaseNode fake_node_;
  size_t sz_;
  allocator_type alloc_;

  void push_back_default();

public:

  List();

  List(size_t count);

  List(size_t count, const T &elem);

  List(const allocator &alloc);

  List(size_t count, const allocator &alloc);

  List(size_t count, const T &elem, const allocator &alloc);

  List(const List<T, allocator> &other);

  List(List<T, allocator> &&other);

  List &operator=(const List<T, allocator> &other);

  List &operator=(List<T, allocator> &&other);

  ~List();

  size_t size();

  size_t size() const;

  void push_back(const T &other);

  void push_front(const T &other);

  void pop_back();

  void pop_front();

  allocator_type get_allocator();

  allocator_type get_allocator() const {
    auto alloc = alloc_;
    return alloc;
  };


  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin();

  const_iterator cend();

  const_iterator cbegin() const;

  const_iterator cend() const;

  reverse_iterator rbegin();

  reverse_iterator rend();

  const_reverse_iterator crbegin();

  const_reverse_iterator crend();

  const_reverse_iterator rbegin() const;

  const_reverse_iterator rend() const;

  const_reverse_iterator crbegin() const;

  const_reverse_iterator crend() const;

  template <typename ...Args>
  void insert(const const_iterator &iter, Args &&... args);

  void erase(const const_iterator &iter);

  void clear() {
    clean();
  }

  void splice(const_iterator pos, List<T, allocator> &list, iterator iter) {
    iter.node_->next_->prev_ = iter.node_->prev_;
    iter.node_->prev_->next_ = iter.node_->next_;
    pos.node_->prev_->next_ = iter.node_;
    iter.node_->prev_ = pos.node_->prev_;
    pos.node_->prev_ = iter.node_;
    iter.node_->next_ = pos.node_;
    --list.sz_;
    ++sz_;
  }

private:
  void clean();
};

template <typename T, class allocator>
List<T, allocator>::List(size_t count, const T &elem, const allocator &alloc): sz_(0), alloc_(alloc) {
  for (; sz_ < count; ++sz_) {
    try {
      push_front(elem);
    } catch (...) {
      clean();
      throw;
    }
  }
  --sz_;
}

template <typename T, class allocator>
List<T, allocator>::List(): fake_node_(), sz_(0), alloc_() {}

template <typename T, class allocator>
List<T, allocator>::List(size_t count): fake_node_(), sz_(0), alloc_() {
  while (sz_ < count) {
    try {
      push_back_default();
    } catch (...) {
      clean();
      throw;
    }
  }
}

template <typename T, class allocator>
List<T, allocator>::List(size_t count, const T &elem): fake_node_(), sz_(0), alloc_() {
  while (sz_ < count) {
    try {
      push_back_default();
    } catch (...) {
      clean();
      throw;
    }
  }
}


template <typename T, class allocator>
List<T, allocator>::List(size_t count, const allocator &alloc): fake_node_(), sz_(0), alloc_(alloc) {
  while (sz_ < count) {
    try {
      push_back_default();
    } catch (...) {
      clean();
      throw;
    }
  }
}


template <typename T, class allocator>
List<T, allocator>::List(const allocator &alloc): sz_(0), alloc_(alloc) {}

template <typename T, class allocator>
List<T, allocator>::List(const List<T, allocator> &other): sz_(0),
                                                           alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(
                                                                   other.alloc_)) {
  for (auto &x: other) {
    try {
      push_back(x);
    } catch (...) {
      clean();
      throw;
    }
  }
}

template <typename T, class allocator>
List<T, allocator>::List(List<T, allocator> &&other): sz_(other.sz_),
                                                      alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(
                                                              other.alloc_)) {
  fake_node_.next_ = other.fake_node_.next_;
  fake_node_.prev_ = other.fake_node_.prev_;
  other.fake_node_.prev_ = reinterpret_cast<Node *>(&other.fake_node_);
  other.fake_node_.next_ = reinterpret_cast<Node *>(&other.fake_node_);
  other.sz_ = 0;
}

template <typename T, class allocator>
List<T, allocator> &List<T, allocator>::operator=(const List<T, allocator> &other) {
  if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
    allocator_type new_alloc(other.alloc_);
    BaseNode new_fake_node;
    size_t local_sz = 0;
    for (auto &x: other) {
      try {
        List::Node *node = new_alloc.allocate(1);
        try {
          new(node) Node(x);
        } catch (...) {
          new_alloc.deallocate(node, 1);
          throw;
        }
        node->next_ = reinterpret_cast<Node *>(&new_fake_node);
        node->prev_ = new_fake_node.prev_;
        new_fake_node.prev_->next_ = node;
        new_fake_node.prev_ = node;
        ++local_sz;
      } catch (...) {
        for (size_t i = 0; i < local_sz; ++i) {
          auto iter = iterator(new_fake_node.next_);
          erase(iter);
        }
        throw;
      }
    }
    clean();
    alloc_ = (other.alloc_);
    fake_node_ = new_fake_node;
    fake_node_.next_->prev_ = reinterpret_cast<Node *>(&fake_node_);
    fake_node_.prev_->next_ = reinterpret_cast<Node *>(&fake_node_);
    sz_ = local_sz;
    return *this;
  }
  auto copy = List<T, allocator>(other);
  clean();
  fake_node_ = copy.fake_node_;
  fake_node_.next_->prev_ = reinterpret_cast<Node *>(&fake_node_);
  fake_node_.prev_->next_ = reinterpret_cast<Node *>(&fake_node_);
  sz_ = other.sz_;
  copy.sz_ = 0;
  copy.fake_node_.prev_ = copy.fake_node_.next_ = nullptr;
  return *this;
}

template <typename T, class allocator>
List<T, allocator> &List<T, allocator>::operator=(List<T, allocator> &&other) {
  clean();
  fake_node_.next_ = other.fake_node_.next_;
  fake_node_.prev_ = other.fake_node_.prev_;
  other.fake_node_.next_->prev_ = reinterpret_cast<Node *>(&fake_node_);
  other.fake_node_.prev_->next_ = reinterpret_cast<Node *>(&fake_node_);
  other.fake_node_.next_ = other.fake_node_.prev_ = reinterpret_cast<Node *>(&other.fake_node_);
  sz_ = other.sz_;
  if (other.sz_ == 0) {
    fake_node_.prev_ = reinterpret_cast<Node*>(&fake_node_);
    fake_node_.next_ = reinterpret_cast<Node*>(&fake_node_);
  }
  other.sz_ = 0;
  return *this;
}

template <typename T, class allocator>
List<T, allocator>::~List() {
  clean();
}

template <typename T, class allocator>
size_t List<T, allocator>::size() {
  return sz_;
}

template <typename T, class allocator>
size_t List<T, allocator>::size() const {
  return sz_;
}


template <typename T, class allocator>
void List<T, allocator>::push_back_default() {
  List::Node *node = alloc_.allocate(1);
  try {
    new(node) Node();
  } catch (...) {
    alloc_.deallocate(node, 1);
    throw;
  }
  node->next_ = reinterpret_cast<Node *>(&fake_node_);

  node->prev_ = fake_node_.prev_;

  fake_node_.prev_->next_ = node;

  fake_node_.prev_ = node;

  ++sz_;
}

template <typename T, class allocator>
void List<T, allocator>::push_back(const T &other) {
  List::Node *node = alloc_.allocate(1);
  try {
    std::allocator_traits<allocator_type>(alloc_, node, other);
  } catch (...) {
    alloc_.deallocate(node, 1);
    throw;
  }
  node->next_ = reinterpret_cast<Node *>(&fake_node_);

  node->prev_ = fake_node_.prev_;

  fake_node_.prev_->next_ = node;

  fake_node_.prev_ = node;

  ++sz_;
}


template <typename T, class allocator>
void List<T, allocator>::push_front(const T &other) {
  Node *node = alloc_.allocate(1);
  try {
    std::allocator_traits<allocator_type>(alloc_, node, other);
  } catch (...) {
    alloc_.deallocate(node, 1);
    throw;
  }
  node->prev_ = reinterpret_cast<List::Node *>(&fake_node_);
  node->next_ = fake_node_.next_;
  fake_node_.next_->prev_ = node;
  fake_node_.next_ = node;
  ++sz_;
}

template <typename T, class allocator>
void List<T, allocator>::pop_back() {
  auto node = fake_node_.prev_;
  node->prev_->next_ = reinterpret_cast<List::Node *>(&fake_node_);
  fake_node_.prev_ = node->prev_;
  (*node).~Node();
  alloc_.deallocate(node, static_cast<size_t>(1));
  --sz_;
}

template <typename T, class allocator>
void List<T, allocator>::pop_front() {
  auto node = fake_node_.next_;
  node->next_->prev_ = reinterpret_cast<List::Node *>(&fake_node_);
  fake_node_.next_ = node->next_;
  (*node).~Node();
  alloc_.deallocate(node, static_cast<size_t>(1));
  --sz_;
}

template <typename T, class allocator>
typename List<T, allocator>::iterator List<T, allocator>::begin() {
  return iterator(fake_node_.next_);
}

template <typename T, class allocator>
typename List<T, allocator>::iterator List<T, allocator>::end() {
  return iterator(reinterpret_cast<List::Node *>(&fake_node_));
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::begin() const {
  return const_iterator(reinterpret_cast<List::Node *>(fake_node_.next_));
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::end() const {
  return const_iterator(const_cast<List::Node *>(reinterpret_cast<const List::Node *>(&fake_node_)));
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::cbegin() {
  return const_iterator(reinterpret_cast<List::Node *>(fake_node_.next_));
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::cend() {
  return const_iterator(&fake_node_);
}


template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::cbegin() const {
  return const_iterator(fake_node_.next_);
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::cend() const {
  return const_iterator(&fake_node_);
}

template <typename T, class allocator>
typename List<T, allocator>::reverse_iterator List<T, allocator>::rbegin() {
  return reverse_iterator(end());
}

template <typename T, class allocator>
typename List<T, allocator>::reverse_iterator List<T, allocator>::rend() {
  return reverse_iterator(begin());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::crbegin() {
  return reverse_iterator(cend());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::crend() {
  return reverse_iterator(cbegin());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::rbegin() const {
  return const_reverse_iterator(end());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::rend() const {
  return const_reverse_iterator(begin());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::crbegin() const {
  return const_reverse_iterator(cend());
}

template <typename T, class allocator>
typename List<T, allocator>::const_reverse_iterator List<T, allocator>::crend() const {
  return const_reverse_iterator(cbegin());
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator &List<T, allocator>::const_iterator::operator++() {
  node_ = reinterpret_cast<Node *>(node_->next_);
  return *this;
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::const_iterator::operator++(int) {
  auto copy = *this;
  node_ = node_->next_;
  return copy;
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator &List<T, allocator>::const_iterator::operator--() {
  node_ = node_->prev_;
  return *this;
}

template <typename T, class allocator>
typename List<T, allocator>::const_iterator List<T, allocator>::const_iterator::operator--(int) {
  auto copy = *this;
  node_ = node_->next_;
  return copy;
}

template <typename T, class allocator>
const T *List<T, allocator>::const_iterator::operator->() {
  return &node_->value_;
}

template <typename T, class allocator>
const T &List<T, allocator>::const_iterator::operator*() {
  return node_->value_;
}

template <typename T, class allocator>
bool List<T, allocator>::const_iterator::operator==(const const_iterator &other) {
  return node_ == other.node_;
}


template <typename T, class allocator>
bool List<T, allocator>::const_iterator::operator!=(const const_iterator &other) {
  return node_ != other.node_;
}

template <typename T, class allocator>
T *List<T, allocator>::iterator::operator->() {
  return &node_->value_;
}

template <typename T, class allocator>
T &List<T, allocator>::iterator::operator*() {
  return node_->value_;
}

template <typename T, class allocator>
typename List<T, allocator>::iterator &List<T, allocator>::iterator::operator++() {
  node_ = node_->next_;
  return *this;
}

template <typename T, class allocator>
typename List<T, allocator>::iterator List<T, allocator>::iterator::operator++(int) {
  auto copy = *this;
  node_ = node_->next_;
  return copy;
}

template <typename T, class allocator>
typename List<T, allocator>::iterator &List<T, allocator>::iterator::operator--() {
  node_ = node_->prev_;
  return *this;
}

template <typename T, class allocator>
typename List<T, allocator>::iterator List<T, allocator>::iterator::operator--(int) {
  auto copy = *this;
  node_ = node_->next_;
  return copy;
}

template <typename T, class allocator>
typename List<T, allocator>::allocator_type List<T, allocator>::get_allocator() {
  return allocator_type(alloc_);
}

template <typename T, class allocator>
template <typename ...Args>
void List<T, allocator>::insert(const const_iterator &iter, Args &&... args) {
  auto node = alloc_.allocate(1);
  try {
    std::allocator_traits<allocator_type>::construct(alloc_, node, std::forward<Args>(args)...);
  } catch (...) {
    alloc_.deallocate(node, 1);
    throw;
  }
  node->next_ = iter.node_;
  iter.node_->prev_->next_ = node;
  node->prev_ = iter.node_->prev_;
  iter.node_->prev_ = node;
  ++sz_;
}

template <typename T, class allocator>
void List<T, allocator>::erase(const const_iterator &iter) {
  iter.node_->prev_->next_ = iter.node_->next_;
  iter.node_->next_->prev_ = iter.node_->prev_;
  (*iter.node_).~Node();
  alloc_.deallocate(iter.node_, 1);
  --sz_;
}

template <typename T, class allocator>
void List<T, allocator>::clean() {
  while (sz_ != 0) {
    erase(begin());
  }
}





template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<Key, Value>>>
class UnorderedMap {
public:
  using NodeType = std::pair<const Key, Value>;
  using allocator_type = typename Alloc::template rebind<typename List<NodeType, Alloc>::iterator>::other;
private:
  using arr_type = typename List<NodeType, Alloc>::iterator;
  using list_type = List<NodeType, Alloc>;
  const static size_t size_in_begin = 8;
  const float max_load_factor_ = 0.75;
  size_t size_of_arr_;
  size_t sz_;
  allocator_type alloc_;
  list_type list_;
  arr_type *arr_;
public:


  struct ConstIterator : protected std::forward_iterator_tag {

  protected:

    typename list_type::iterator iter_;
  public:

    using NodeType = std::pair<const Key, const Value>;
    using iterator_category = std::forward_iterator_tag;
    using value_type = NodeType;
    //typedef size_t difference_type;
    using difference_type = size_t;
    using pointer = const NodeType *;
    using reference = const NodeType &;

    ConstIterator(typename list_type::const_iterator iter) : iter_(
            *(reinterpret_cast<typename list_type::iterator *>(&iter))) {};

    ConstIterator(const ConstIterator &other) = default;

    ConstIterator &operator=(const ConstIterator &other) = default;

    ConstIterator(ConstIterator &&other) = default;

    ConstIterator &operator=(ConstIterator &&other) = default;

    ConstIterator operator++(int) {
      auto copy = *this;
      ++iter_;
      return copy;
    }

    ConstIterator &operator++() {
      ++iter_;
      return *this;
    };

    ConstIterator operator--(int) {
      auto copy = *this;
      --iter_;
      return copy;
    };

    ConstIterator &operator--() {
      --iter_;
      return *this;
    }

    const std::pair<const Key, Value> &operator*() {
      return *iter_;
    }

    const std::pair<const Key, Value> *operator->() {
      return &(*iter_);
    };

    bool operator==(const ConstIterator &other) {
      return iter_ == other.iter_;
    };

    bool operator!=(const ConstIterator &other) {
      return !(*this == other);
    }
  };

  struct Iterator : public ConstIterator {
  public:

    friend class unordered_map;

  public:
    using ConstIterator::iter_;
  public:


    /*using NodeType = std::pair<const Key, Value>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = size_t;
    using pointer = NodeType *;
    using reference = NodeType &;*/

    Iterator(typename list_type::const_iterator iter) : ConstIterator(iter) {};

    Iterator(const Iterator &other) = default;

    Iterator &operator=(const Iterator &other) = default;

    Iterator(Iterator &&other) = default;

    Iterator &operator=(Iterator &&other) = default;

    Iterator operator++(int) {
      auto copy = *this;
      ++iter_;
      return copy;
    };

    Iterator &operator++() {
      ++iter_;
      return *this;
    };

    Iterator operator--(int) {
      auto copy = this;
      --iter_;
      return copy;
    }

    Iterator &operator--() {
      --iter_;
      return *this;
    }

    NodeType &operator*() {
      return *iter_;
    }

    NodeType *operator->() {
      return &(*iter_);
    }
  };



  UnorderedMap();

  UnorderedMap(const UnorderedMap<Key, Value, Hash, Equal, Alloc> &other);

  UnorderedMap(UnorderedMap<Key, Value, Hash, Equal, Alloc> &&other);

  UnorderedMap &operator=(const UnorderedMap<Key, Value, Hash, Equal, Alloc> &other);

  UnorderedMap &operator=(UnorderedMap<Key, Value, Hash, Equal, Alloc> &&other);

  ~UnorderedMap();

  Value &operator[](const Key &key);

  Value &operator[](Key &&key);

  Value &at(const Key &key);

  Value &at(Key &&key);

  Iterator begin();

  Iterator end();

  ConstIterator begin() const;

  ConstIterator end() const;

  ConstIterator cbegin();

  ConstIterator cend();

  ConstIterator cbegin() const;

  ConstIterator cend() const;

  std::pair<Iterator, bool> insert(const NodeType &pair);

  std::pair<Iterator, bool> insert(NodeType &&pair);

  template <class P>
  std::pair<Iterator, bool> insert(P &&elem);

  template <typename InputIterator>
  void insert(InputIterator begin, InputIterator end);



  template <typename... Args>
  std::pair<Iterator, bool> emplace(Args &&... args);


  std::pair<Iterator, bool> emplace(int key, int value) {
    if (sz_ + 1 > max_size()) {
      rehash(sz_ * 2);
    }
    auto solving_of_big_CE_problem = std::pair<char[sizeof(Key)], char[sizeof(Value)]>();
    typename Alloc::template rebind<Key>::other().construct(reinterpret_cast<Key *>(&(solving_of_big_CE_problem.first)),
                                                            std::move(key));
    typename Alloc::template rebind<Value>::other().construct(
            reinterpret_cast<Value *>(&(solving_of_big_CE_problem.second)), std::move(value));
    list_.insert(list_.end(), std::move(*reinterpret_cast<std::pair<Key, Value> *>(&solving_of_big_CE_problem)));
    auto &pair = *(--list_.end());
    auto iter = find_in_arr_list(arr_, size_of_arr_, list_, pair.first);
    if (iter != Iterator(list_.end()) && iter != Iterator(--list_.end())) {
      list_.erase(--list_.end());
      return std::pair<Iterator, bool>(iter, false);
    }
    size_t index = Hash{}(pair.first) % size_of_arr_;
    list_.splice(arr_[index], list_, --list_.end());
    arr_[index] = --arr_[index];
    ++sz_;
    return std::pair<Iterator, bool>(Iterator(arr_[index]), true);
  }


  void erase(Iterator iter);

  void erase(Iterator begin, Iterator end);

  void reserve(size_t new_cap);

  size_t max_size();

  float load_factor();

  float max_load_factor();

  Iterator find(const Key &key);

  size_t size() {
    return sz_;
  }

private:
  Iterator find_in_arr_list(arr_type *arr, size_t arr_size, list_type &list, const Key &key);

  void rehash(size_t new_cup);

  template <typename... Args>
  std::pair<Iterator, bool> emplace_in_arr_list(arr_type *arr, size_t arr_size, list_type &list, Args &&... args);




  std::pair<Iterator, bool> emplace_in_arr_list(arr_type *arr, size_t arr_size, list_type &list,
                                                std::pair<const Key, const Value> &&args);

};

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(): size_of_arr_(size_in_begin), sz_(0), alloc_(),
                                                              list_(alloc_),
                                                              arr_(nullptr) {
  arr_ = reinterpret_cast<arr_type *>(std::allocator_traits<allocator_type>::allocate(
          alloc_, size_in_begin));
  for (size_t i = 0; i < size_of_arr_; ++i) {
    new(&arr_[i]) arr_type(list_.end());
  }
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(const UnorderedMap &other): size_of_arr_(other.size_of_arr_),
                                                                                       sz_(other.sz_),
                                                                                       alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(
                                                                                               other.alloc_)),
                                                                                       list_(std::allocator_traits<typename list_type::allocator_type>::select_on_container_copy_construction(
                                                                                               other.list_.get_allocator())),
                                                                                       arr_(
                                                                                               nullptr) {
  arr_ = reinterpret_cast<arr_type *>(std::allocator_traits<allocator_type>::allocate(
          alloc_, size_of_arr_));
  for (size_t i = 0; i < size_of_arr_; ++i) {
    new(&arr_[i]) arr_type(list_.end());
  }
  for (auto &pair: other) {
    try {
      insert(std::pair<const Key, Value>(pair));
    } catch (...) {
      list_.clear();
      alloc_.deallocate(arr_, size_of_arr_);
      throw;
    }
  }
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(UnorderedMap &&other): size_of_arr_(
        other.size_of_arr_), sz_(other.sz_),
                                                                                  alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(
                                                                                          other.alloc_)),
                                                                                  list_(std::move(other.list_)),
                                                                                  arr_(
                                                                                          other.arr_) {
  other.size_of_arr_ = 0;
  other.sz_ = 0;
  other.arr_ = nullptr;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc> &
UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator=(const UnorderedMap<Key, Value, Hash, Equal, Alloc> &other) {
  allocator_type new_alloc = alloc_;
  if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
    new_alloc = other.alloc_;
  }
  list_type new_list(std::allocator_traits<typename list_type::allocotor_type>(other.list_.get_allocator()));
  arr_type *new_arr = nullptr;
  try {
    new_arr = std::allocator_traits<allocator_type>::allocate(new_alloc, other.size_of_arr_);
  } catch (...) {
    new_list.~List();
    throw;
  }
  for (size_t i = 0; i < size_of_arr_; ++i) {
    new(&new_arr[i]) arr_type(list_.end());
  }
  try {
    insert_in_arr_list(new_arr, new_list, other.begin(), other.end());
  } catch (...) {
    new_list.~List();
    std::allocator_traits<allocator_type>::deallocate(new_alloc, arr_, other.size_of_arr_);
    throw;
  }
  std::allocator_traits<allocator_type>::deallocate(alloc_, arr_, size_of_arr_);
  arr_ = new_arr;
  alloc_ = new_alloc;
  list_ = std::move(new_list);
  sz_ = other.sz_;
  size_of_arr_ = other.size_of_arr_;
  return *this;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc> &
UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator=(UnorderedMap<Key, Value, Hash, Equal, Alloc> &&other) {
  list_.clear();
  std::allocator_traits<allocator_type>::deallocate(alloc_, arr_, size_of_arr_);
  arr_ = other.arr_;
  list_ = std::move(other.list_);
  size_of_arr_ = other.size_of_arr_;
  sz_ = other.sz_;
  if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
    alloc_ = other.alloc_;
  }
  other.arr_ = nullptr;
  other.sz_ = 0;
  other.size_of_arr_ = 0;
  return *this;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::~UnorderedMap<Key, Value, Hash, Equal, Alloc>() {
  if (arr_ == nullptr) return;
  std::allocator_traits<allocator_type>::deallocate(alloc_, arr_, size_of_arr_);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator[](const Key &key) {
  Iterator iter = find(key);
  if (iter != this->end()) {
    return iter->second;
  }
  return emplace(key).first.iter_->second;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator[](Key &&key) {
  Iterator iter = find(key);
  if (iter != this->end()) {
    return iter->second;
  }
  return emplace(std::move(key), Value()).first.iter_->second;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::at(const Key &key) {
  Iterator iter = find(key);
  if (iter != this->end()) {
    return iter->second;
  }
  throw std::out_of_range("vietnamese flashbacks");
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::at(Key &&key) {
  Iterator iter = find(key);
  if (iter != this->end()) {
    return iter->second;
  }
  throw std::out_of_range("vietnamese flashbacks");
}


template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>::begin() {
  return Iterator(list_.begin());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>::end() {
  return Iterator(list_.end());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::begin() const {
  return ConstIterator(list_.begin());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::end() const {
  return ConstIterator(list_.end());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::cbegin() {
  return ConstIterator(list_.begin());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::cend() {
  return ConstIterator(list_.end());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::cbegin() const {
  return ConstIterator(list_.begin());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::ConstIterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::cend() const {
  return ConstIterator(list_.end());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(const
                                                     NodeType &pair) {
  return emplace(pair);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(
        NodeType &&pair) {
  return emplace(std::move(pair));
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template <class P>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(
        P &&elem) {
  return emplace(std::move(elem));
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template <typename InputIterator>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(InputIterator begin, InputIterator end) {
  while (begin != end) {
    emplace(*(begin++));
  }
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
size_t UnorderedMap<Key, Value, Hash, Equal, Alloc>::max_size() {
  return static_cast<size_t>(max_load_factor_ * size_of_arr_);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
float UnorderedMap<Key, Value, Hash, Equal, Alloc>::load_factor() {
  return (static_cast<float >(sz_) / static_cast<float>(size_of_arr_));
}


template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
float UnorderedMap<Key, Value, Hash, Equal, Alloc>::max_load_factor() {
  return max_load_factor_;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::reserve(size_t new_cup) {
  if (max_size() < new_cup) {
    rehash(new_cup);
  }
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template <typename ...Args>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace(
        Args &&...args) {
  if (static_cast<float>(sz_ + 1) / static_cast<float>(size_of_arr_) > max_load_factor_) {
    rehash(sz_ * 2);
  }
  return emplace_in_arr_list(arr_, size_of_arr_, list_, std::forward<Args>(args)...);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template <typename ...Args>

std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace_in_arr_list(arr_type *arr, size_t arr_size, list_type &list,
                                                                  Args &&... args) {
  list_.insert(list.end(), std::forward<Args>(args)...);
  auto &pair = *(--list_.end());
  auto iter = find_in_arr_list(arr, arr_size, list, pair.first);
  if (iter != Iterator(list.end()) && iter != Iterator(--list.end())) {
    list.erase(--list.end());
    return std::pair<Iterator, bool>(iter, false);
  }
  size_t index = Hash{}(pair.first) % arr_size;
  list_.splice(arr[index], list_, --list.end());
  arr[index] = --arr[index];
  ++sz_;
  return std::pair<Iterator, bool>(Iterator(arr[index]), true);
}

/*
template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace_in_arr_list<int, int>(arr_type *arr, size_t arr_size, list_type &list,
                                                                  int &&key, int &&value) {
  auto solving_of_big_CE_problem = std::pair<char[sizeof(Key)], char[sizeof(Value)]>();
  typename Alloc::template rebind<Key>::other().construct(reinterpret_cast<Key *>(&(solving_of_big_CE_problem.first)),
                                                          std::move(key));
  typename Alloc::template rebind<Value>::other().construct(
          reinterpret_cast<Value *>(&(solving_of_big_CE_problem.second)), std::move(value));
  list_.insert(list.end(), std::move(*reinterpret_cast<std::pair<Key, Value> *>(&solving_of_big_CE_problem)));
  auto &pair = *(--list_.end());
  auto iter = find_in_arr_list(arr, arr_size, list, pair.first);
  if (iter != Iterator(list.end()) && iter != Iterator(--list.end())) {
    list.erase(--list.end());
    return std::pair<Iterator, bool>(iter, false);
  }
  size_t index = Hash{}(pair.first) % arr_size;
  list_.splice(arr[index], list_, --list.end());
  arr[index] = --arr[index];
  ++sz_;
  return std::pair<Iterator, bool>(Iterator(arr[index]), true);
}
*/
template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace_in_arr_list(arr_type *arr, size_t arr_size, list_type &list,
                                                                  std::pair<const Key, const Value> &&args) {
  list_.insert(list.end(), std::move(args));
  auto &pair = *(--list_.end());
  auto iter = find_in_arr_list(arr, arr_size, list, pair.first);
  if (iter != Iterator(list.end()) && iter != Iterator(--list.end())) {
    list.erase(--list.end());
    return std::pair<Iterator, bool>(iter, false);
  }
  size_t index = Hash{}(pair.first) % arr_size;
  list_.splice(arr[index], list_, --list.end());
  arr[index] = --arr[index];
  ++sz_;
  return std::pair<Iterator, bool>(Iterator(arr[index]), true);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>::find(
        const Key &key) {
  return find_in_arr_list(arr_, size_of_arr_, list_, key);
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::Iterator
UnorderedMap<Key, Value, Hash, Equal, Alloc>::find_in_arr_list(
        arr_type *arr, size_t arr_size, list_type &list, const Key &key) {
  auto index = Hash{}(key) % arr_size;
  auto iter = arr[index];
  while (iter != list.end() && (Hash{}(iter->first) % arr_size) == index) {
    if (Equal{}(iter->first, key)) {
      return Iterator(iter);
    }
    ++iter;
  }
  return Iterator(list.end());
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(Iterator iter) {
  auto index = Hash{}(iter.iter_->first) % size_of_arr_;
  if (arr_[index] != iter.iter_) {
    list_.erase(iter.iter_);
    --sz_;
    return;
  }
  auto copy_iter = iter.iter_;
  if (Hash{}((++copy_iter)->first) % size_of_arr_ != index) {
    arr_[index] = (list_.end());
  } else {
    arr_[index] = (++copy_iter);
  }
  list_.erase(iter.iter_);
  --sz_;
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(Iterator begin, Iterator end) {
  auto copy = begin;
  while (copy != end) {
    erase(copy++);
  }
}

template <typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::rehash(size_t new_cup) {
  size_t new_size = (static_cast<float>(new_cup > max_size() ? new_cup * max_load_factor_ : max_size()) * 3); // strange number not in N
  auto new_arr = std::allocator_traits<allocator_type>::allocate(alloc_, new_size);
  auto new_list = list_type(list_.get_allocator());
  for (size_t i = 0; i < new_size; ++i) {
    new(&new_arr[i]) arr_type(new_list.end());
  }
  for (auto iter = list_.begin(), end = list_.end(); iter != end; iter = list_.begin()) {
    auto index = Hash{}(iter->first) % new_size;
    new_list.splice(new_arr[index], list_, iter);
    new_arr[index]--;
  }
  for (size_t i = 0; i < new_size; ++i) {
    if (new_arr[i] == new_list.end()) {
      new_arr[i] = list_.end();
    }
  }
  std::allocator_traits<allocator_type>::deallocate(alloc_, arr_, size_of_arr_);
  arr_ = new_arr;
  size_of_arr_ = new_size;
  list_ = std::move(new_list);
}

#endif //UNTITLED_UNORDERED_MAP_H
