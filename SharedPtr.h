//
// Created by diffraction on 21/05/2022.
//

#include <iterator>
#include <memory>
#include <exception>


#ifndef UNTITLED_SMART_POINTERS2_H
#define UNTITLED_SMART_POINTERS2_H

struct unused;  // forward declaration only


template <class Container>
using const_ref_if_copy_constructible = typename std::conditional<
        std::is_copy_constructible<Container>::value,
        Container const &,
        unused>::type;

template <class Container>
using const_ref_if_not_copy_constructible = typename std::conditional<
        std::is_copy_constructible<Container>::value,
        unused,
        Container const &>::type;










template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

template <typename T, typename ... Args>
SharedPtr<T> makeShared(Args &&... args);

template <typename T, typename Allocator, typename ... Args>
SharedPtr<T> allocateShared(const Allocator &allocator, Args &&... args);

template <typename T>
class SharedPtr {

  template <typename U, typename ... Args>
  friend SharedPtr<U> makeShared(Args &&... args);

  template <typename U, typename Allocator, typename ... Args>
  friend SharedPtr<U> allocateShared(const Allocator &allocator, Args &&... args);

  template <typename U>
  friend class WeakPtr;

  template <typename U>
  friend class SharedPtr;

  struct Base {
      size_t shared_count_;
      size_t weak_count_;

      Base() : shared_count_(1), weak_count_(0) {};

      virtual T *give_pointer() = 0;

      virtual void destruct_obj() = 0;

      virtual void dealloc_struct() = 0;
  };

  template <typename Allocator = std::allocator<T>>
  struct Obj : public Base {
      T value_;
      Allocator allocator_;

      // constructors

      Obj() = default;

      explicit Obj(const Allocator &alloc) : allocator_(alloc) {};

      template <typename ...Args>
      explicit Obj(Args &&... args): value_(std::forward<Args>(args)...) {};

      template <typename ...Args>
      explicit Obj(const Allocator &alloc, Args &&... args): value_(std::forward<Args>(args)...), allocator_(alloc) {};

      // in case T not have copy constructor

      Obj(const_ref_if_copy_constructible<T> &obj, const Allocator &allocator = Allocator()) : value_(obj),
                                                                                                        allocator_(allocator) {};

      Obj(const_ref_if_not_copy_constructible<T> &obj, const Allocator &allocator = Allocator()) : value_(
              std::move(obj)),
                                                                                                            allocator_(
                                                                                                                    allocator) {};

      // other method

      T *give_pointer() override {
        return &value_;
      }

      void destruct_obj() override {
        using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
        allocator_type another_alloc = allocator_;
        std::allocator_traits<allocator_type>::destroy(another_alloc, &value_);
      }

      void dealloc_struct() override {
        using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Obj<Allocator>>;
        allocator_type another_alloc = allocator_;
        std::allocator_traits<allocator_type>::deallocate(another_alloc, this, 1);
      }
  };

  template <typename Deleter = std::default_delete<T>, typename Allocator = std::allocator<T>>
  struct Ptr : public Base {
      T *ptr_;
      Deleter deleter_;
      Allocator allocator_;

      Ptr(T *pointer, const Deleter &deleter = Deleter(), const Allocator &allocator = Allocator()) :
              ptr_(pointer), deleter_(deleter), allocator_(allocator) {};

      T *give_pointer() override {
        return ptr_;
      }

      void destruct_obj() override {
        deleter_(ptr_);
      }

      void dealloc_struct() override {
        using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Ptr < Deleter, Allocator>>;
        alloc_type copy = allocator_;
        std::allocator_traits<alloc_type>::deallocate(copy, this, 1);
      }
  };

  Base *main_point_;

  void clean_shared() {
    if (main_point_ == nullptr) {
      return;
    }
    --main_point_->shared_count_;
    if (main_point_->shared_count_ == 0) {
      main_point_->destruct_obj();
      if (main_point_->weak_count_ == 0) {
        main_point_->dealloc_struct();
      }
    }
    main_point_ = nullptr;
  };

public:

  template <typename Deleter = std::default_delete<T>, typename Allocator = std::allocator<T>>
  explicit SharedPtr(T *ptr, const Deleter &del = Deleter(), const Allocator &alloc = Allocator()): main_point_(nullptr) {
    using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Ptr < Deleter, Allocator>>;
    alloc_type copy = alloc;
    auto point = std::allocator_traits<alloc_type>::allocate(copy, 1);
    std::allocator_traits<alloc_type>::construct(copy, point, ptr, del, alloc);
    main_point_ = point;
  };

  SharedPtr(): main_point_(nullptr) {
  };

  template <typename U>
  SharedPtr(const SharedPtr<U> &other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (main_point_ == nullptr) {
      return;
    }
    ++main_point_->shared_count_;
  };

  SharedPtr(const SharedPtr<T> &other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (main_point_ == nullptr) {
      return;
    }
    ++main_point_->shared_count_;
  };


  template <typename U>
  SharedPtr<T> &operator=(const SharedPtr<U> &other) {
    if (main_point_ == reinterpret_cast<Base*>(other.main_point_)) {
      return *this;
    }
    clean_shared();
    main_point_ = reinterpret_cast<Base*> (other.main_point_);
    ++main_point_->shared_count_;
    return *this;
  }

  SharedPtr<T> &operator=(const SharedPtr<T> &other) {
    if (main_point_ == reinterpret_cast<Base*>(other.main_point_)) {
      return *this;
    }
    clean_shared();
    main_point_ = reinterpret_cast<Base*> (other.main_point_);
    ++main_point_->shared_count_;
    return *this;
  }

  template <typename U>
  explicit SharedPtr(SharedPtr<U> &&other): main_point_(other.main_point_) {
    other.main_point_ = nullptr;
  };

  template <typename U>
  SharedPtr<T> &operator=(SharedPtr<U> &&other) {
    if (main_point_ != reinterpret_cast<Base*>(other.main_point_)) {
      clean_shared();
      main_point_ = reinterpret_cast<Base*>(other.main_point_);
      ++main_point_->shared_count_;
    }
    other.clean_shared();
    return *this;
  }

  ~SharedPtr() {
    clean_shared();
  }

private: // constructors for make/allocate shared / lock in WeakPtr

  /*template <typename Allocator, typename ... Args>
  SharedPtr(const Allocator &allocator, Args &&... args): main_point_(nullptr) {
    using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Obj<Allocator>>;
    alloc_type copy = allocator;
    auto point = std::allocator_traits<alloc_type>::allocate(copy, 1);
    std::allocator_traits<alloc_type>::construct(copy, point, std::forward<Args>(args)..., allocator);
    main_point_ = point;
  }*/

  explicit SharedPtr(const WeakPtr<T>& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (!main_point_) return;
    ++main_point_->shared_count_;
  }

public:
  size_t use_count() {
    return main_point_->shared_count_;
  };

  size_t use_count() const {
    return main_point_->shared_count_;
  };

  void reset() {
    clean_shared();
  }

  void swap(SharedPtr<T> &other) {
    auto copy = main_point_;
    main_point_ = other.main_point_;
    other.main_point_ = copy;
  }

  T *operator->() {
    return main_point_->give_pointer();
  }

  T &operator*() {
    return *(main_point_->give_pointer());
  }

  const T *operator->() const {
    return main_point_->give_pointer();
  }

  const T &operator*() const {
    return *(main_point_->give_pointer());
  }

  T *get() {
    if (main_point_ == nullptr) {
      return nullptr;
    }
    return main_point_->give_pointer();
  }

  const T *get() const {
    if (main_point_ == nullptr) {
      return nullptr;
    }
    return main_point_->give_pointer();
  }

  template <typename Deleter = std::default_delete<T>, typename Allocator = std::allocator<T>>
  void reset(T *ptr, const Deleter &deleter = Deleter(), const Allocator &allocator = Allocator()) {
    auto copy = SharedPtr<T>(ptr, deleter, allocator);
    clean_shared();
    main_point_ = copy.main_point_;
    copy.main_point_ = nullptr;
  }
};



/// WeakPtr





template <typename T>
class WeakPtr {

  template <typename U>
  friend class WeakPtr;

  template <typename U>
  friend class SharedPtr;

  using Base = typename SharedPtr<T>::Base;
  Base* main_point_;

  void clear_ptr() {
    if (main_point_ == nullptr) {
      return;
    }
    --main_point_->weak_count_;
    if (main_point_->weak_count_ == 0 && main_point_->shared_count_ == 0) {
      main_point_->dealloc_struct();
    }
    main_point_ = nullptr;
  }


public:

  WeakPtr(): main_point_(nullptr) {};

  template <typename U>
  WeakPtr(const SharedPtr<U>& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (!main_point_) {
      return;
    }
    ++main_point_->weak_count_;
  }

  WeakPtr(const SharedPtr<T>& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (!main_point_) {
      return;
    }
    ++main_point_->weak_count_;
  }

  template <typename U>
  WeakPtr(const WeakPtr<U>& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (!main_point_) {
      return;
    }
    ++main_point_->weak_count_;
  }

  WeakPtr(const WeakPtr<T>& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (!main_point_) {
      return;
    }
    ++main_point_->weak_count_;
  }


  template <typename U>
  explicit WeakPtr(WeakPtr<U>&& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (main_point_) {
      ++main_point_->weak_count_;
    }
    other.clear_ptr();
  }

  explicit WeakPtr(WeakPtr<T>&& other): main_point_(reinterpret_cast<Base*>(other.main_point_)) {
    if (main_point_) {
      ++main_point_->weak_count_;
    }
    other.clear_ptr();
  }

  template <typename U>
  WeakPtr<T>& operator=(const WeakPtr<U>& other) {
    if (main_point_ == other.main_point_) {
      return *this;
    }
    clear_ptr();
    main_point_ = other.main_point_;
    ++main_point_->weak_count_;
    return *this;
  }

  WeakPtr<T>& operator=(const WeakPtr<T>& other) {
    if (main_point_ == other.main_point_) {
      return *this;
    }
    clear_ptr();
    main_point_ = other.main_point_;
    ++main_point_->weak_count_;
    return *this;
  }

  template <typename U>
  WeakPtr<T>& operator=(const SharedPtr<U>& other) {
    if (main_point_ == reinterpret_cast<Base*>(other.main_point_)) {
      return *this;
    }
    clear_ptr();
    main_point_ = reinterpret_cast<Base *>(other.main_point_);
    ++main_point_->weak_count_;
    return *this;
  }

  WeakPtr<T>& operator=(const SharedPtr<T>& other) {
    if (main_point_ == reinterpret_cast<Base*>(other.main_point_)) {
      return *this;
    }
    clear_ptr();
    main_point_ = reinterpret_cast<Base *>(other.main_point_);
    ++main_point_->weak_count_;
    return *this;
  }



  template <typename U>
  WeakPtr<T>& operator=(WeakPtr<U>&& other) {
    if (main_point_ == other.main_point_) {
      other.clear_ptr();
      return *this;
    }
    clear_ptr();
    main_point_ = other.main_point_;
    ++main_point_->weak_count_;
    other.clear_ptr();
    return *this;
  }

  WeakPtr<T>& operator=(WeakPtr<T>&& other) {
    if (main_point_ == other.main_point_) {
      other.clear_ptr();
      return *this;
    }
    clear_ptr();
    main_point_ = other.main_point_;
    ++main_point_->weak_count_;
    other.clear_ptr();
    return *this;
  }

  ~WeakPtr() {
    clear_ptr();
  }




  bool expired() {
    return main_point_->shared_count_ == 0;
  }

  bool expired() const {
    return main_point_->shared_count_ == 0;
  }

  SharedPtr<T> lock() {
    if (expired()) {
      throw std::runtime_error("It's dead(\n");
    }
    return SharedPtr<T>(*this);
  };

  size_t use_count() {
    return main_point_->shared_count_;
  };

  const SharedPtr<T> lock() const {
    if (expired()) {
      throw std::runtime_error("It's dead(\n");
    }
    return SharedPtr<T>(*this);
  };

  size_t use_count() const {
    return main_point_->shared_count_;
  };
};

template <typename T, typename ... Args>
SharedPtr<T> makeShared(Args &&... args) {
  return allocateShared<T>(std::allocator<T>(), std::forward<Args>(args)...);
}


template <typename T, typename Allocator, typename ... Args>
SharedPtr<T> allocateShared(const Allocator &allocator, Args &&... args) {
  using alloc_type = typename std::allocator_traits<Allocator>::template rebind_alloc<typename SharedPtr<T>::template Obj<Allocator>>;
  alloc_type copy = allocator;
  auto point = std::allocator_traits<alloc_type>::allocate(copy, 1);
  std::allocator_traits<alloc_type>::construct(copy, point, allocator, std::forward<Args>(args)...);
  SharedPtr<T> shared;
  shared.main_point_ = point;
  return shared;
}



#endif //UNTITLED_SMART_POINTERS2_H
