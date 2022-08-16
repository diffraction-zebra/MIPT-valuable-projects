//
/// Created by diffraction on 16/06/2022.
//

#include <exception>
#include <type_traits>
#include <algorithm>

#ifndef UNTITLED_VARIANT_H
#define UNTITLED_VARIANT_H

template <class T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

template <typename ...Types>
class Variant;

template <typename T, typename ...Types>
class VariantAlternative;

template <typename T, typename ...Ts>
auto &get(Variant<Ts...> &v);

template <size_t N, typename ...Ts>
auto &get(Variant<Ts...> &v);

template <typename T, typename ...Ts>
const auto &get(const Variant<Ts...> &v);

template <size_t N, typename ...Ts>
const auto &get(const Variant<Ts...> &v);

template <typename T, typename ...Ts>
auto &&get(Variant<Ts...> &&v);

template <size_t N, typename ...Ts>
auto &&get(Variant<Ts...> &&v);

/// Index by type

template <size_t N, bool Any, typename T, typename Head, typename ...Tail>
struct index_by_type_impl {
    static_assert(!(std::is_same_v<T, Head> && Any),
                  "Same types in list");
    static constexpr size_t value = std::is_same_v<T, Head> ?
                                    N :
                                    index_by_type_impl<N + 1, Any || std::is_same_v<T, Head>, T, Tail...>::value;
};

template <size_t N, bool Any, typename T, typename Last>
struct index_by_type_impl<N, Any, T, Last> {
    static_assert(!(std::is_same_v<T, Last> && Any),
                  "Same types in list");
    static_assert(std::is_same_v<T, Last> || Any,
                  "type is not a member of initialization list");
    static constexpr size_t value = N;
};

template <typename T, typename ...Types>
static constexpr size_t index_by_type =
        index_by_type_impl<0, false, T, Types...>::value;


/// Type by index

template <size_t N, bool Any, typename Head, typename ...Types>
struct type_by_index_impl {
    using value = std::conditional_t<N == 0, Head,
            typename type_by_index_impl<N - 1, N == 0 || Any, Types...>::value>;
};

template <size_t N, bool Any, typename Last>
struct type_by_index_impl<N, Any, Last> {
    static_assert(N == 0 || Any, "N more than number of types");
    using value = Last;
};

template <size_t N, typename ...Types>
using type_by_index = typename type_by_index_impl<N, false, Types...>::value;


/// VariadicUnion


template <typename ...Args>
union VariadicUnion {

    VariadicUnion() {};

    VariadicUnion(const VariadicUnion &) {};

    VariadicUnion(VariadicUnion &&) {};

    VariadicUnion &operator=(const VariadicUnion &) { return *this; }

    VariadicUnion &operator=(VariadicUnion &&) { return *this; }

    ~VariadicUnion() {};

    template <size_t N>
    auto &get() {
      throw std::bad_typeid();
    }

    template <typename T>
    void put(const T &) {

    }

    template <typename T>
    void put(T &&) {

    }

    template <typename T>
    void change(const T &) {

    }

    template <typename T>
    void change(T &&) {

    }

    void clear(size_t) {
      throw std::bad_typeid();
    }

    void copy_constructor(size_t, const VariadicUnion &) {

      throw std::bad_typeid();
    }

    void move_constructor(size_t, VariadicUnion &) {

      throw std::bad_typeid();
    }

    template <typename T, typename ...As>
    void emplace(As &&...) {

      throw "emplace reach end of recursion";
    }

};

template <typename Head, typename ...Types>
union VariadicUnion<Head, Types...> {
    VariadicUnion<Types...> tail_;
    Head head_;


    VariadicUnion() {};

    VariadicUnion(const VariadicUnion &) {};

    VariadicUnion(VariadicUnion &&) {};

    VariadicUnion &operator=(const VariadicUnion &) { return *this; }

    VariadicUnion &operator=(VariadicUnion &&) { return *this; }

    ~VariadicUnion() {};

    template <size_t N>
    auto &get() {
      if constexpr(N == 0) {
        return head_;
      } else {
        return tail_.template get<N - 1>();
      }
    }


    template <typename T>
    void put(T &&value) {
      if constexpr (std::is_same_v<T, Head>) {
        new((std::launder(&head_))) T(std::move(value));
      } else {
        tail_.template put<T>(std::move(value));
      }
    }

    template <typename T>
    void put(const T &value) {
      if constexpr (std::is_same_v<T, Head>) {
        new(const_cast<std::remove_const_t<Head> *>(std::launder(&head_))) T(value);
      } else {
        tail_.template put<T>(value);
      }
    }

    template <typename T, typename ...Args>
    void emplace(Args &&... args) {
      if constexpr (std::is_same_v<T, Head>) {
        new(const_cast<std::remove_const_t<Head> *>(std::launder(&head_)))
                T(std::forward<Args>(args)...);
      } else {
        tail_.template emplace<T, Args...>(std::forward<Args>(args)...);
      }
    }

    template <typename T>
    void change(const T &value) {
      if constexpr (std::is_same_v<T, Head>) {
        head_ = value;
      } else {
        tail_.template change<T>(value);
      }
    }

    template <typename T>
    void change(T &&value) {
      if constexpr (std::is_same_v<T, Head>) {
        head_ = std::move(value);
      } else {
        tail_.template change<T>(std::move(value));
      }
    }

    void clear(size_t N) {
      if (N == 0) {
        head_.~Head();
      } else {
        tail_.clear(N - 1);
      }
    }

    void copy_constructor(size_t index, const VariadicUnion &other) {
      if (index == 0) {
        new(&head_) Head(other.head_);
        return;
      }
      tail_.copy_constructor(index - 1, other.tail_);
    }

    void move_constructor(size_t index, VariadicUnion &other) {
      if (index == 0) {
        new(std::launder(&head_)) Head(std::move(other.head_));
        return;
      }
      tail_.move_constructor(index - 1, other.tail_);
    }

};



/// VariantAlternative

template <typename T, typename ...Types>
struct VariantAlternative {

    using Derived = Variant<Types...>;

    static constexpr size_t index = index_by_type<T, Types...>;

    VariantAlternative() = default;

    VariantAlternative(const VariantAlternative &) = delete;

    ~VariantAlternative() = default;

    VariantAlternative(const T &value) {
      auto ptr = static_cast<Derived *>(this);
      ptr->storage_.template put<T>(value);
      ptr->index_ = index;
    }

    Variant<Types...> &operator=(const T &value) {
      auto ptr = static_cast<Derived *>(this);
      if (!ptr->is_empty_) {
        if (ptr->index_ == index) {
          ptr->storage_.template change<T>(value);
          return *ptr;
        }
        ptr->storage_.clear(ptr->index_);
      }
      ptr->storage_.template put<T>(value);
      ptr->index_ = index;
      return *ptr;
    }

    Variant<Types...> &operator=(T &&value) {
      auto ptr = static_cast<Derived *>(this);
      if (!ptr->is_empty_) {
        if (ptr->index_ == index) {
          ptr->storage_.template change<T>(std::move(value));
          return *ptr;
        }
        ptr->storage_.clear(ptr->index_);
      }
      ptr->storage_.template put<T>(std::move(value));
      ptr->index_ = index;
      return *ptr;
    }

};

/// is_assignable_once

template <typename U, size_t Num, typename Head, typename ...Tail>
static constexpr size_t is_assignable_once_impl =
        std::is_assignable_v<Head, U> ?
        is_assignable_once_impl < U, Num + 1, Tail
...> :
is_assignable_once_impl<U, Num, Tail...>;

template <typename U, size_t Num, typename Tail>
static constexpr size_t is_assignable_once_impl<U, Num, Tail> =
        std::is_assignable_v<Tail, U> ?
        Num + 1 : Num;

template <typename U, typename ...Types>
static constexpr bool is_assignable_once = (is_assignable_once_impl<U,
        0, Types...> == 1);


/// is_constructable_once


template <typename U, size_t Num, typename Head, typename ...Tail>
static constexpr size_t is_constructable_once_impl = std::is_constructible_v<Head, U> ?
                                                     is_constructable_once_impl < U, Num + 1, Tail
...> :
is_constructable_once_impl<U, Num, Tail...>;

template <typename U, size_t Num, typename Tail>
static constexpr size_t is_constructable_once_impl<U, Num, Tail> = std::is_constructible_v<Tail, U> ?
                                                                   Num + 1 : Num;

template <typename U, typename ...Types>
static constexpr bool is_constructable_once = (is_constructable_once_impl<U,
        0, Types...> == 1);


template <typename ...Types>
class VariantStorage {
protected:
  bool is_empty_ = false;
  size_t index_ = 0;
  VariadicUnion<Types...> storage_ = VariadicUnion<Types...>();
};


///Variant

template <typename ...Types>
class Variant : public VariantStorage<Types...>,
                protected VariantAlternative<Types, Types...> ... {

  template <typename T, typename ...Ts, size_t index>
  friend bool holds_alternative(const Variant<Ts...> &v);

  template <typename T, typename ...Ts>
  friend
  class VariantAlternative;

  template <size_t N, typename ...Ts>
  friend auto &get(Variant<Ts...> &v);

  template <size_t N, typename ...Ts>
  friend auto &&get(Variant<Ts...> &&v);

  template <size_t N, typename ...Ts>
  friend const auto &get(const Variant<Ts...> &v);

private:
  using VariantStorage<Types...>::storage_;
  using VariantStorage<Types...>::is_empty_;
  using VariantStorage<Types...>::index_;
  using first_type = type_by_index<0, Types...>;

  template <typename U, typename Type>
  std::enable_if_t<std::is_constructible_v<Type, U>, void>
  Variant_constructor_helper(const U &value) {
    static constexpr size_t index = index_by_type<Type, Types...>;
    storage_.template put<Type>(value);
    index_ = index;
  }

  template <typename U, typename Type>
  std::enable_if_t<!std::is_constructible_v<Type, U>, void>
  Variant_constructor_helper(const U &value) {};

  template <typename U, typename Head, typename ...Ts>
  void Variant_constructor(const U &value) {
    if constexpr (std::is_constructible_v<Head, U>) {
      Variant_constructor_helper<U, Head>(value);
      return;
    }
    Variant_constructor<U, Ts...>(value);
  };

  template <typename U>
  void Variant_constructor(const U &) {
    throw "construction followed bad way";
  };

  template <typename U, typename Type>
  std::enable_if_t<std::is_constructible_v<Type, U>, void>
  Variant_assign_helper(const U &value) {
    static constexpr size_t index = index_by_type<Type, Types...>;
    if (index == index_) {
      storage_.template change<Type>(value);
      return;
    }
    storage_.clear(index_);
    storage_.template put<Type>(value);
    index_ = index;
  }

  template <typename U, typename Type>
  std::enable_if_t<!std::is_constructible_v<Type, U>, void>
  Variant_assign_helper(const U &value) {};

  template <typename U, typename Head, typename ...Ts>
  void Variant_assign(const U &value) {
    if constexpr (std::is_constructible_v<Head, U>) {
      Variant_assign_helper<U, Head>(value);
      return;
    }
    Variant_assign < U, Ts...>(value);
  }

  template <typename U>
  void Variant_assign(const U &) {
    throw "assign followed bad way";
  };

public:
  using VariantAlternative<Types, Types...>::VariantAlternative...;


  Variant() : VariantAlternative<first_type, Types...>(std::move<first_type>(first_type())) {};

  template <typename U, typename = std::enable_if_t<is_constructable_once<U, Types...>>>
  Variant(const U &value) {
    Variant_constructor<U, Types...>(value);
  }

  template <typename U, typename = std::enable_if_t<is_constructable_once<U, Types...>>>
  Variant &operator=(const U &value) {
    if (is_empty_) {
      Variant_constructor<U, Types...>(value);
    } else {
      Variant_assign<U, Types...>(value);
    }
    return *this;
  }

  Variant(const Variant &other) : VariantStorage<Types...>(), VariantAlternative<Types, Types...>()... {
    is_empty_ = other.is_empty_;
    index_ = other.index_;
    storage_.copy_constructor(index_, other.storage_);
  }

  Variant(Variant &&other) {
    is_empty_ = other.is_empty_;
    index_ = other.index_;
    if (is_empty_) {
      return;
    }
    storage_.move_constructor(index_, other.storage_);
    other.is_empty_ = true;
  }

  Variant &operator=(const Variant &other) {
    storage_.clear(index_);
    is_empty_ = other.is_empty_;
    if (!is_empty_) {
      storage_.copy_constructor(index_, other.storage_);
    }
    return *this;
  }

  Variant &operator=(Variant &&other) {
    storage_.clear(index_);
    is_empty_ = other.is_empty_;
    index_ = other.index_;
    if (!is_empty_) {
      storage_.move_constructor(index_, other.storage_);
    }
    other.is_empty_ = true;
    return *this;
  }

  template <typename T, typename ...Args>
  auto &emplace(Args &&... args) {
    storage_.clear(index_);
    storage_.template emplace<T, Args...>(std::forward<Args>(args)...);
    index_ = index_by_type<T, Types...>;
    return get<T>(*this);
  }

  template <typename T, typename U>
  auto &emplace(std::initializer_list<U> list) {
    storage_.clear(index_);
    storage_.template emplace<T, decltype(list.begin()), decltype(list.begin())>(list.begin(), list.end());
    index_ = index_by_type<T, Types...>;
    return get<T>(*this);
  }

  template <size_t N, typename ...Args>
  auto &emplace(Args &&...args) {
    return emplace<type_by_index<N, Types...>, Args...>(std::forward<Args>(args)...);
  }

  ~Variant() {
    storage_.clear(index_);
  }
};

/// Functions

// get

// lvalue
template <size_t N, typename ...Ts>
auto &get(Variant<Ts...> &v) {
  if (v.index_ == N) {
    return v.storage_.template get<N>();
  }
  throw std::bad_typeid();
}

template <typename T, typename ...Ts>
auto &get(Variant<Ts...> &v) {
  constexpr auto index = index_by_type<T, Ts...>;
  return get<index, Ts...>(v);
}

// const

template <size_t N, typename ...Ts>
const auto &get(const Variant<Ts...> &v) {
  if (v.index_ == N) {
    return (const_cast<Variant<Ts...> *>(&v))->storage_.template get<N>();
  }
  throw std::bad_typeid();
}

template <typename T, typename ...Ts>
const auto &get(const Variant<Ts...> &v) {
  constexpr auto index = index_by_type<T, Ts...>;
  return get<index, Ts...>(v);
}

// rvalue

template <size_t N, typename ...Ts>
auto &&get(Variant<Ts...> &&v) {
  if (v.index_ == N) {
    return std::move(v.storage_.template get<N>());
  }
  throw std::bad_typeid();
}

template <typename T, typename ...Ts>
auto &&get(Variant<Ts...> &&v) {
  constexpr auto index = index_by_type<T, Ts...>;
  return get<index, Ts...>(std::move(v));
}

template <typename T, typename ...Ts, size_t index = index_by_type<T, Ts...>>
bool holds_alternative(const Variant<Ts...> &v) {
  return index == v.index_;
}

#endif //UNTITLED_VARIANT_H