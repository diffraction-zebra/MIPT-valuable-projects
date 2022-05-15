//
// Created by diffraction on 25/03/2022.
//

#ifndef UNTITLED_DEQUE_2_0_H
#define UNTITLED_DEQUE_2_0_H

#include<iterator>

template <class T>
class Deque {
private:
  T **arr_ = nullptr;
  static const int lines_in_begin_ = 3; // in case constructing of empty Deque
  static const int size_of_line = 16;
  int lines_; //number of lines
  int sz_;
  int first_; // index of first line
  int last_; // index of last line
  int first_elem_; // index of first element in first line
  int last_elem_; // index of last element in last line
  void arr_clean();

  void clean(T **arr, int first, int last, int first_elem,
             int last_elem); // calling destructors for block [first; last - 1] on [first_elem; last_elem - 1]
  T *allocate();

  void deallocate(T *point);

  void reallocate();

public:
  Deque();

  Deque(const Deque<T> &other);

  Deque(int size);

  Deque(int size, const T &elem);

  Deque<T> &operator=(const Deque<T> &other);

  ~Deque();

  size_t size();

  size_t size() const;

  T &operator[](int index);

  const T &operator[](int index) const;

  T &at(int index);

  const T &at(int index) const;

  void push_back(T elem);

  void push_front(T elem);

  void pop_back();

  void pop_front();

  class const_iterator;

  class iterator;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  const_iterator cbegin();

  const_iterator cend();

  const_iterator cbegin() const;

  const_iterator cend() const;

  void insert(iterator iter, const T &elem);

  void erase(iterator iter);
};

template <class T>
class Deque<T>::const_iterator {
protected:
  T **arr_;
  int line_;
  int elem_;
  friend Deque<T>;
public:
  const_iterator(T **arr, int line, int elem) : arr_(arr), line_(line), elem_(elem) {};

  const_iterator(const const_iterator &iter) : arr_(iter.arr_), line_(iter.line_), elem_(iter.elem_) {}

  const_iterator &operator=(const const_iterator &other) = default;

  ~const_iterator() = default;

  const_iterator &operator++();

  const_iterator operator++(int);

  const_iterator &operator--();

  int operator-(const_iterator iter);

  const_iterator operator--(int);

  const_iterator &operator+=(int a);

  const_iterator &operator-=(int a);

  const_iterator operator+(int a);

  const_iterator operator-(int a);

  bool operator<(const const_iterator &other);

  bool operator>(const const_iterator &other);

  bool operator<=(const const_iterator &other);

  bool operator>=(const const_iterator &other);

  bool operator==(const const_iterator &other);

  bool operator!=(const const_iterator &other);

  bool operator<(const const_iterator &other) const;

  bool operator>(const const_iterator &other) const;

  bool operator<=(const const_iterator &other) const;

  bool operator>=(const const_iterator &other) const;

  bool operator==(const const_iterator &other) const;

  bool operator!=(const const_iterator &other) const;

  const T &operator*() {
    return arr_[line_][elem_];
  }

  const T *operator->() {
    return &(arr_[line_][elem_]);
  }
};

template <class T>
class Deque<T>::iterator : public Deque<T>::const_iterator {
private:
  friend Deque<T>;
  using Deque<T>::const_iterator::arr_;
  using Deque<T>::const_iterator::line_;
  using Deque<T>::const_iterator::elem_;
public:
  iterator(T **arr, int line, int elem) : Deque<T>::const_iterator(arr, line, elem) {};

  iterator(const iterator &other) : Deque<T>::const_iterator(other.arr_, other.line_, other.elem_) {};

  iterator &operator=(const iterator &other) = default;

  ~iterator() = default;

  iterator &operator++();

  iterator operator++(int);

  iterator &operator--();

  int operator-(iterator iter);

  iterator operator--(int);

  iterator &operator+=(int);

  iterator &operator-=(int);

  iterator operator+(int);

  iterator operator-(int);

  T &operator*() {
    return arr_[line_][elem_];
  }

  T *operator->() {
    return &(arr_[line_][elem_]);
  }
};

template <class T>
int Deque<T>::const_iterator::operator-(const_iterator iter) {
  return (line_ - iter.line_) * size_of_line + elem_ - iter.elem_;
}

template <class T>
typename Deque<T>::const_iterator &Deque<T>::const_iterator::operator++() {
  if (elem_ == size_of_line - 1) {
    elem_ = 0;
    ++line_;
  } else {
    ++elem_;
  }
  return *this;
}

template <class T>
typename Deque<T>::const_iterator &Deque<T>::const_iterator::operator--() {
  if (elem_ == 0) {
    elem_ = size_of_line - 1;
    --line_;
  } else {
    --elem_;
  }
  return *this;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::const_iterator::operator++(int) {
  typename Deque<T>::const_iterator iter(*this);
  if (elem_ == size_of_line - 1) {
    elem_ = 0;
    ++line_;
  } else {
    ++elem_;
  }
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::const_iterator::operator--(int) {
  typename Deque<T>::const_iterator iter(*this);
  if (elem_ == 0) {
    elem_ = size_of_line - 1;
    --line_;
  } else {
    --elem_;
  }
  return iter;
}

template <class T>
typename Deque<T>::const_iterator &Deque<T>::const_iterator::operator+=(int a) {
  int real_index = line_ * size_of_line + elem_ + a;
  line_ = real_index / size_of_line;
  elem_ = real_index % size_of_line;
  return *this;
}

template <class T>
typename Deque<T>::const_iterator &Deque<T>::const_iterator::operator-=(int a) {
  int real_index = line_ * size_of_line + elem_ - a;
  line_ = real_index / size_of_line;
  elem_ = real_index % size_of_line;
  return *this;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::const_iterator::operator+(int a) {
  typename Deque<T>::const_iterator iter(*this);
  iter += a;
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::const_iterator::operator-(int a) {
  typename Deque<T>::const_iterator iter(*this);
  iter -= a;
  return iter;
}

template <class T>
bool Deque<T>::const_iterator::operator<(const typename Deque<T>::const_iterator &other) const {
  return (line_ * size_of_line + elem_) < (other.line_ * size_of_line + other.elem_);
}

template <class T>
bool Deque<T>::const_iterator::operator>(const typename Deque<T>::const_iterator &other) const {
  return other < *this;
}

template <class T>
bool Deque<T>::const_iterator::operator==(const typename Deque<T>::const_iterator &other) const {
  return !(other < *this || other > *this);
}

template <class T>
bool Deque<T>::const_iterator::operator<=(const typename Deque<T>::const_iterator &other) const {
  return other > *this || other == *this;
}

template <class T>
bool Deque<T>::const_iterator::operator>=(const typename Deque<T>::const_iterator &other) const {
  return other < *this || other == *this;
}

template <class T>
bool Deque<T>::const_iterator::operator!=(const typename Deque<T>::const_iterator &other) const {
  return other > *this || other < *this;
}

template <class T>
bool Deque<T>::const_iterator::operator<(const typename Deque<T>::const_iterator &other) {
  return other > *this;
}

template <class T>
bool Deque<T>::const_iterator::operator>(const typename Deque<T>::const_iterator &other) {
  return other < *this;
}

template <class T>
bool Deque<T>::const_iterator::operator<=(const typename Deque<T>::const_iterator &other) {
  return other >= *this;
}

template <class T>
bool Deque<T>::const_iterator::operator>=(const typename Deque<T>::const_iterator &other) {
  return other < *this || other == *this;
}

template <class T>
bool Deque<T>::const_iterator::operator==(const typename Deque<T>::const_iterator &other) {
  return other == *this;
}

template <class T>
bool Deque<T>::const_iterator::operator!=(const typename Deque<T>::const_iterator &other) {
  return other != *this;
}

template <class T>
int Deque<T>::iterator::operator-(iterator iter) {
  return (line_ - iter.line_) * size_of_line + elem_ - iter.elem_;
}

template <class T>
typename Deque<T>::iterator &Deque<T>::iterator::operator++() {
  if (elem_ == size_of_line - 1) {
    elem_ = 0;
    ++line_;
  } else {
    ++elem_;
  }
  return *this;
}

template <class T>
typename Deque<T>::iterator &Deque<T>::iterator::operator--() {
  if (elem_ == 0) {
    elem_ = size_of_line - 1;
    --line_;
  } else {
    --elem_;
  }
  return *this;
}

template <class T>
typename Deque<T>::iterator Deque<T>::iterator::operator++(int) {
  typename Deque<T>::iterator iter(*this);
  if (elem_ == size_of_line - 1) {
    elem_ = 0;
    ++line_;
  } else {
    ++elem_;
  }
  return iter;
}

template <class T>
typename Deque<T>::iterator Deque<T>::iterator::operator--(int) {
  typename Deque<T>::iterator iter(*this);
  if (elem_ == 0) {
    elem_ = size_of_line - 1;
    --line_;
  } else {
    --elem_;
  }
  return iter;
}

template <class T>
typename Deque<T>::iterator &Deque<T>::iterator::operator+=(int a) {
  int real_index = line_ * size_of_line + elem_ + a;
  line_ = real_index / size_of_line;
  elem_ = real_index % size_of_line;
  return *this;
}

template <class T>
typename Deque<T>::iterator &Deque<T>::iterator::operator-=(int a) {
  int real_index = line_ * size_of_line + elem_ - a;
  line_ = real_index / size_of_line;
  elem_ = real_index % size_of_line;
  return *this;
}

template <class T>
typename Deque<T>::iterator Deque<T>::iterator::operator+(int a) {
  typename Deque<T>::iterator iter(*this);
  iter += a;
  return iter;
}

template <class T>
typename Deque<T>::iterator Deque<T>::iterator::operator-(int a) {
  typename Deque<T>::iterator iter(*this);
  iter -= a;
  return iter;
}

template <class T>
T *Deque<T>::allocate() {
  return reinterpret_cast<T *>(new uint8_t[sizeof(T) * size_of_line]);
}

template <class T>
void Deque<T>::deallocate(T *pointer) {
  delete[] reinterpret_cast<uint8_t *>(pointer);
}

template <class T>
void Deque<T>::clean(T **arr, int first, int last, int first_elem, int last_elem) {
  for (int i = first; i < last; ++i) {
    for (int j = first_elem; j < last_elem; ++j) {
      arr[i][j].~T();
    }
  }
}

template <class T>
void Deque<T>::arr_clean() {
  if (first_ == last_) {
    clean(arr_, first_, first_ + 1, first_elem_, last_elem_ + 1);
    return;
  }
  clean(arr_, first_, first_ + 1, first_elem_, size_of_line);
  clean(arr_, first_ + 1, last_, 0, size_of_line);
  clean(arr_, last_, last_ + 1, 0, last_elem_ + 1);
}

template <class T>
void Deque<T>::reallocate() {
  int new_lines = lines_ * 3;
  T **new_arr = new T *[new_lines];
  for (int i = 0; i < lines_; ++i) {
    try {
      new_arr[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  for (int i = lines_ * 2; i < new_lines; ++i) {
    try {
      new_arr[i] = allocate();
    } catch (...) {
      for (int j = 0; j < lines_; ++j) {
        deallocate(new_arr[j]);
      }
      for (int j = lines_ * 2; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  int limit = lines_ * 2;
  for (int i = lines_; i < limit; ++i) {
    new_arr[i] = arr_[i - lines_];
  }
  delete[] arr_;
  arr_ = new_arr;
  lines_ = new_lines;
  first_ += (lines_ / 3);
  last_ += (lines_ / 3);
}

template <class T>
Deque<T>::Deque(): lines_(lines_in_begin_), sz_(0), first_(0), last_(0), first_elem_(0), last_elem_(0) {
  arr_ = new T *[lines_in_begin_];
  for (int i = 0; i < lines_in_begin_; ++i) {
    try {
      arr_[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(arr_[j]);
      }
      delete[] arr_;
      throw;
    }
  }
}

template <class T>
Deque<T>::Deque(const Deque<T> &other): arr_(nullptr), lines_(other.lines_), sz_(other.sz_),
                                        first_(other.first_),
                                        last_(other.last_), first_elem_(other.first_elem_),
                                        last_elem_(other.last_elem_) {
  if (other.sz_ == 0) {
    arr_ = new T *[lines_in_begin_];
    for (int i = 0; i < lines_in_begin_; ++i) {
      try {
        arr_[i] = allocate();
      } catch (...) {
        for (int j = 0; j < i; ++j) {
          deallocate(arr_[j]);
        }
        delete[] arr_;
        throw;
      }
    }
    lines_ = lines_in_begin_;
    sz_ = 0;
    return;
  }
  int new_lines = 3 * ((other.sz_ + size_of_line - 1) / size_of_line);
  int new_first = new_lines / 3;
  int new_last = 2 * (new_lines / 3) - 1;
  int new_last_elem = (other.sz_ % size_of_line) - 1;
  if (new_last_elem == -1) {
    new_last_elem = size_of_line - 1;
  }
  arr_ = new T *[new_lines];
  for (int i = 0; i < new_lines; ++i) {
    try {
      arr_[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(arr_[j]);
      }
      delete[] arr_;
      throw;
    }
  }
  const_iterator iter = other.begin();
  for (int i = first_; i < last_; ++i)
    for (int j = 0; j < size_of_line; ++j) {
      try {
        new(&arr_[i][j]) T(*iter);
        ++iter;
      } catch (...) {
        clean(arr_, first_, i, 0, size_of_line);
        clean(arr_, i, i + 1, 0, j);
        for (int k = 0; k < i; ++k) {
          deallocate(arr_[j]);
        }
        delete[] arr_;
        throw;
      }
    }
  for (int i = 0; i <= new_last_elem; ++i) {
    try {
      new(&arr_[new_last][i]) T(*iter);
      ++iter;
    } catch (...) {
      clean(arr_, first_, last_, 0, size_of_line);
      clean(arr_, last_, last_ + 1, 0, i);
      for (int j = 0; j < i; ++j) {
        deallocate(arr_[j]);
      }
      delete[] arr_;
      throw;
    }
  }
  sz_ = other.sz_;
  lines_ = new_lines;
  first_ = new_first;
  last_ = new_last;
  first_elem_ = 0;
  last_elem_ = new_last_elem;
}

template <class T>
Deque<T>::Deque(int size): arr_(nullptr), lines_(3 * ((size + size_of_line - 1) / size_of_line)), sz_(size),
                           first_(lines_ / 3), last_((lines_ / 3) * 2 - 1), first_elem_(0),
                           last_elem_(((sz_ % size_of_line) - 1)) {
  if (last_elem_ == -1) {
    last_elem_ = size_of_line - 1;
  }
  arr_ = new T *[lines_];
  for (int i = 0; i < lines_; ++i) {
    try {
      arr_[i] = allocate();
    } catch (...) {
      for (int j = 0; j < lines_; ++j) {
        deallocate(arr_[i]);
      }
      delete[] arr_;
      throw;
    }
  }
  if (first_ == last_) {
    for (int i = 0; i <= last_elem_; ++i) {
      try {
        new(&arr_[first_][i]) T();
      } catch (...) {
        clean(arr_, first_, first_ + 1, 0, i);
        for (int k = 0; k < lines_; ++k) {
          deallocate(arr_[k]);
        }
        delete[] arr_;
        throw;
      }
    }
    return;
  }
  for (int j = first_; j < last_; ++j) {
    for (int i = 0; i < size_of_line; ++i) {
      try {
        new(&arr_[j][i]) T();
      } catch (...) {
        clean(arr_, first_, j, 0, size_of_line);
        clean(arr_, j, j + 1, 0, i);
        for (int k = 0; k < lines_; ++k) {
          deallocate(arr_[k]);
        }
        delete[] arr_;
        throw;
      }
    }
  }
  for (int i = 0; i < last_elem_; ++i) {
    try {
      new(&arr_[last_][i]) T();
    } catch (...) {
      clean(arr_, first_, last_, 0, size_of_line);
      clean(arr_, last_, last_ + 1, 0, i);
      for (int k = 0; k < lines_; ++k) {
        deallocate(arr_[k]);
      }
      delete[] arr_;
      throw;
    }
  }
}

template <class T>
Deque<T>::Deque(int size, const T &elem): arr_(nullptr), lines_(3 * ((size + size_of_line - 1) / size_of_line)),
                                          sz_(size),
                                          first_(lines_ / 3), last_((lines_ / 3) * 2 - 1), first_elem_(0),
                                          last_elem_(((sz_ % size_of_line) - 1)) {
  if (last_elem_ == -1) {
    last_elem_ = size_of_line - 1;
  }
  arr_ = new T *[lines_];
  for (int i = 0; i < lines_; ++i) {
    try {
      arr_[i] = allocate();
    } catch (...) {
      for (int j = 0; j < lines_; ++j) {
        deallocate(arr_[i]);
      }
      delete[] arr_;
      throw;
    }
  }
  if (first_ == last_) {
    for (int i = 0; i <= last_elem_; ++i) {
      try {
        new(&arr_[first_][i]) T(elem);
      } catch (...) {
        clean(arr_, first_, first_ + 1, 0, i);
        for (int k = 0; k < lines_; ++k) {
          deallocate(arr_[i]);
        }
        delete[] arr_;
        throw;
      }
    }
    return;
  }
  for (int j = first_; j < last_; ++j) {
    for (int i = 0; i < size_of_line; ++i) {
      try {
        new(&arr_[j][i]) T(elem);
      } catch (...) {
        clean(arr_, first_, j, 0, size_of_line);
        clean(arr_, j, j + 1, 0, i);
        for (int k = 0; k < lines_; ++k) {
          deallocate(arr_[k]);
        }
        delete[] arr_;
        throw;
      }
    }
  }
  for (int i = 0; i < last_elem_; ++i) {
    try {
      new(&arr_[last_][i]) T(elem);
    } catch (...) {
      clean(arr_, first_, last_, 0, size_of_line);
      clean(arr_, last_, last_ + 1, 0, i);
      for (int k = 0; k < lines_; ++k) {
        deallocate(arr_[k]);
      }
      delete[] arr_;
      throw;
    }
  }
}

template <class T>
Deque<T> &Deque<T>::operator=(const Deque<T> &other) {
  if (other.sz_ == 0) {
    T **new_arr = new T *[lines_in_begin_];
    for (int i = 0; i < lines_in_begin_; ++i) {
      try {
        new_arr[i] = allocate();
      } catch (...) {
        for (int j = 0; j < i; ++j) {
          deallocate(new_arr[j]);
        }
        delete[] new_arr;
        throw;
      }
    }
    arr_clean();
    for (int i = 0; i < lines_; ++i) {
      deallocate(arr_[i]);
    }
    delete[] arr_;
    arr_ = new_arr;
    lines_ = lines_in_begin_;
    sz_ = 0;
    return *this;
  }
  int new_lines = 3 * ((other.sz_ + size_of_line - 1) / size_of_line);
  int new_first = new_lines / 3;
  int new_last = 2 * (new_lines / 3) - 1;
  int new_last_elem = ((other.sz_ % size_of_line) - 1);
  if (new_last_elem == -1) {
    new_last_elem = size_of_line - 1;
  }
  T **new_arr = new T *[new_lines];
  for (int i = 0; i < new_lines; ++i) {
    try {
      new_arr[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  const_iterator iter = other.begin();
  for (int i = first_; i < last_; ++i)
    for (int j = 0; j < size_of_line; ++j) {
      try {
        new(&new_arr[i][j]) T(*iter);
        ++iter;
      } catch (...) {
        clean(new_arr, first_, i, 0, size_of_line);
        clean(new_arr, i, i + 1, 0, j);
        for (int k = 0; k < i; ++k) {
          deallocate(new_arr[j]);
        }
        delete[] new_arr;
        throw;
      }
    }
  for (int i = 0; i <= new_last_elem; ++i) {
    try {
      new(&new_arr[new_last][i]) T(*iter);
      ++iter;
    } catch (...) {
      clean(new_arr, first_, last_, 0, size_of_line);
      clean(new_arr, last_, last_ + 1, 0, i);
      for (int j = 0; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  arr_clean();
  for (int i = 0; i < lines_; ++i) {
    deallocate(arr_[i]);
  }
  delete[] arr_;
  arr_ = new_arr;
  sz_ = other.sz_;
  lines_ = new_lines;
  first_ = new_first;
  last_ = new_last;
  first_elem_ = 0;
  last_elem_ = new_last_elem;
  return *this;
}

template <class T>
Deque<T>::~Deque<T>() {
  arr_clean();
  for (int i = 0; i < lines_; ++i) {
    deallocate(arr_[i]);
  }
  delete[] arr_;
}

template <class T>
size_t Deque<T>::size() {
  return static_cast<size_t>(sz_);
}

template <class T>
size_t Deque<T>::size() const {
  return static_cast<size_t>(sz_);
}

template <class T>
T &Deque<T>::operator[](int index) {
  iterator iter = begin();
  iter += index;
  return *iter;
}

template <class T>
const T &Deque<T>::operator[](int index) const {
  const_iterator iter = cbegin();
  iter += index;
  return *iter;
}

template <class T>
T &Deque<T>::at(int index) {
  if (index >= sz_) {
    throw std::out_of_range{"out_of_range"};
  }
  return (*this)[index];
}

template <class T>
const T &Deque<T>::at(int index) const {
  if (index >= sz_) {
    throw std::out_of_range{"out_of_range"};
  }
  return (*this)[index];
}

template <class T>
void Deque<T>::push_back(T elem) {
  if (sz_ != 0 && last_ == lines_ - 1 && last_elem_ == size_of_line - 1) {
    reallocate();
  }
  int cur_line = last_;
  int cur_elem = last_elem_ + 1;
  if (last_elem_ == size_of_line - 1) {
    ++cur_line;
    cur_elem = 0;
  }
  if (sz_ == 0) { // in this case last_ and last_elem_ are invalid
    cur_line = lines_ / 3;
    cur_elem = 0;
  }
  new(&arr_[cur_line][cur_elem]) T(elem);
  ++sz_;
  last_ = cur_line;
  last_elem_ = cur_elem;
  if (sz_ == 1) {
    first_ = last_;
    first_elem_ = last_elem_;
  }
}

template <class T>
void Deque<T>::push_front(T elem) {
  if (sz_ != 0 && first_ == 0 && first_elem_ == 0) {
    reallocate();
  }
  int cur_line = first_;
  int cur_elem = first_elem_ - 1;
  if (first_elem_ == 0) {
    --cur_line;
    cur_elem = size_of_line - 1;
  }
  if (sz_ == 0) { // in this case first_ and first_elem_ are invalid
    cur_line = lines_ / 3;
    cur_elem = 0;
  }
  new(&arr_[cur_line][cur_elem]) T(elem);
  ++sz_;
  first_ = cur_line;
  first_elem_ = cur_elem;
  if (sz_ == 1) {
    last_ = first_;
    last_elem_ = first_elem_;
  }
}

template <class T>
void Deque<T>::pop_back() {
  arr_[last_][last_elem_].~T();
  --sz_;
  if (last_elem_ == 0) {
    --last_;
    last_elem_ = size_of_line;
  }
  --last_elem_;
}

template <class T>
void Deque<T>::pop_front() {
  arr_[first_][first_elem_].~T();
  --sz_;
  if (first_elem_ == size_of_line - 1) {
    ++first_;
    first_elem_ = -1;
  }
  ++first_elem_;
}

template <class T>
typename Deque<T>::iterator Deque<T>::begin() {
  typename Deque<T>::iterator iter(arr_, first_, first_elem_);
  return iter;
}

template <class T>
typename Deque<T>::iterator Deque<T>::end() {
  typename Deque<T>::iterator iter(arr_, last_, last_elem_);
  ++iter;
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::begin() const {
  typename Deque<T>::const_iterator iter(arr_, first_, first_elem_);
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::end() const {
  typename Deque<T>::const_iterator iter(arr_, last_, last_elem_);
  ++iter;
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::cend() {
  typename Deque<T>::const_iterator iter(arr_, last_, last_elem_);
  ++iter;
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::cbegin() {
  typename Deque<T>::const_iterator iter(arr_, first_, first_elem_);
  return iter;
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::cend() const {
  return (*this).end();
}

template <class T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const {
  return (*this).begin();
}

template <class T>
void Deque<T>::insert(iterator iter, const T &elem) {
  if (last_ == lines_ - 1 && last_elem_ == size_of_line - 1) {
    reallocate();
  }
  int first = (iter.line_ == first_ ? first_elem_ : 0);
  int new_lines = last_ - iter.line_ + 1 + (last_elem_ + 1) / size_of_line;
  T **new_arr = new T *[new_lines];
  for (int i = 0; i < new_lines; ++i) {
    try {
      new_arr[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }


  for (int i = first; i < iter.elem_; ++i) {
    try {
      new(&new_arr[0][i]) T(arr_[iter.line_][i]);
    } catch (...) {
      for (int j = first; j < i; ++j) {
        new_arr[0][j].~T();
      }
      for (int j = 0; j < new_lines; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  try {
    new(&new_arr[0][iter.elem_]) T(elem);
  } catch (...) {
    for (int i = first_; i < iter.elem_; ++i) {
      new_arr[0][i].~T();
    }
    for (int j = 0; j < new_lines; ++j) {
      deallocate(new_arr[j]);
    }
    delete[] new_arr;
    throw;
  }
  iterator copy = iter;
  int limit = last_elem_ - iter.elem_ + (last_ - iter.line_) * size_of_line + 1;
  for (int count = 0, cur_line = 0, cur_elem = iter.elem_ + 1; count < limit; ++count, ++copy, ++cur_elem) {
    if (cur_elem == size_of_line) {
      cur_elem = 0;
      ++cur_line;
    }
    try {
      new(&new_arr[cur_line][cur_elem]) T(*copy);
    } catch (...) {
      for (int i = first_; i <= iter.elem_; ++i) {
        new_arr[0][i].~T();
      }
      for (int new_count = 0, new_cur_line = 0, new_cur_elem = iter.elem_ + 1; new_count < count; ++count, ++new_cur_elem) {
        if (new_cur_elem == size_of_line) {
          new_cur_elem = 0;
          ++new_cur_line;
        }
        new_arr[cur_line][cur_elem].~T();
      }
      for (int j = 0; j < new_lines; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  for (int i = 0; i < new_lines; ++i) {
    clean(arr_, i + iter.line_,
          i + iter.line_ + 1, (i + iter.line_ == first_ ? first_elem_ : 0), (i + iter.line_ == last_ ? last_elem_ + 1
                                                                                                     : size_of_line));
    deallocate(arr_[i + iter.line_]);
    arr_[i + iter.line_] = new_arr[i];
  }
  delete[] new_arr;
  ++sz_;
  ++last_elem_;
  if (last_elem_ == size_of_line) {
    last_elem_ = 0;
    ++last_;
  }
}

template <class T>
void Deque<T>::erase(iterator iter) {
  int first = (iter.line_ == first_ ? first_elem_ : 0);
  int new_lines = last_ - iter.line_ + 1;
  if (last_elem_ == 0) {
    --new_lines;
  }
  T **new_arr = new T *[new_lines];
  for (int i = 0; i < new_lines; ++i) {
    try {
      new_arr[i] = allocate();
    } catch (...) {
      for (int j = 0; j < i; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  for (int i = first; i < iter.elem_; ++i) {
    try {
      new(&new_arr[0][i]) T(arr_[iter.line_][i]);
    } catch (...) {
      for (int j = first; j < i; ++j) {
        new_arr[0][j].~T();
      }
      for (int j = 0; j < new_lines; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  iterator copy = iter + 1;
  int limit = last_elem_ - iter.elem_ + (last_ - iter.line_) * size_of_line;
  for (int count = 0, cur_line = 0, cur_elem = iter.elem_; count < limit; ++count, ++copy, ++cur_elem) {
    if (cur_elem == size_of_line) {
      cur_elem = 0;
      ++cur_line;
    }
    try {
      new(&new_arr[cur_line][cur_elem]) T(*copy);
    } catch (...) {
      for (int i = first_; i < iter.elem_; ++i) {
        new_arr[0][i].~T();
      }
      for (int new_count = 0, new_cur_line = 0, new_cur_elem = iter.elem_; new_count < count; ++count, ++new_cur_elem) {
        if (new_cur_elem == size_of_line) {
          new_cur_elem = 0;
          ++new_cur_line;
        }
        new_arr[cur_line][cur_elem].~T();
      }
      for (int j = 0; j < new_lines; ++j) {
        deallocate(new_arr[j]);
      }
      delete[] new_arr;
      throw;
    }
  }
  for (int i = 0; i < new_lines; ++i) {
    clean(arr_, i + iter.line_,
          i + iter.line_ + 1, (i + iter.line_ == first_ ? first_elem_ : 0), (i + iter.line_ == last_ ? last_elem_ + 1
                                                                                                     : size_of_line));
    delete[] arr_[i + iter.line_];
    arr_[i + iter.line_] = new_arr[i];
  }
  delete[] new_arr;
  --sz_;
  --last_elem_;
  if (last_elem_ == -1) {
    last_elem_ = size_of_line - 1;
    --last_;
  }
}

#endif //UNTITLED_DEQUE_2_0_H
