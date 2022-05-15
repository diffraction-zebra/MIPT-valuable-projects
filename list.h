#define  allocator_type typename allocator:: template rebind<typename List<T, allocator>::Node>::other

template <typename T, class allocator = std::allocator<T>>
class List {
private:
  struct Node;

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

      Node(const T &other) : value_(other) {
        prev_ = nullptr;
        next_ = nullptr;
      }

      Node() : value_() {
        prev_ = nullptr;
        next_ = nullptr;
      }

      ~Node() = default;
  };

  struct const_iterator : protected std::bidirectional_iterator_tag {
  protected:
    Node *node_;
  public:

    friend struct List::Node;
    friend struct List::BaseNode;

    template <class U, class allocator_other>
    friend
    class List;

    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T *;
    using reference = const T &;

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

    using value_type = T;
    using pointer = T *;
    using reference = T &;

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

  List &operator=(const List<T, allocator> &other);

  ~List();

  size_t size();

  size_t size() const;

  void push_back(const T &other);

  void push_front(const T &other);

  void pop_back();

  void pop_front();

  allocator_type get_allocator();

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

  void insert(const const_iterator &iter, const T &elem);

  void erase(const const_iterator &iter);

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
    new(node) Node(other);
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
    new(node) Node(other);
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
allocator_type List<T, allocator>::get_allocator() {
  return allocator_type(alloc_);
}

template <typename T, class allocator>
void List<T, allocator>::insert(const const_iterator &iter, const T &elem) {
  auto node = alloc_.allocate(1);
  try {
    new(node) Node(elem);
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


#endif
