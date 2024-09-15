#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <assert.h>

template <typename T>
class Vector {
 private:
  size_t capacity_ = 0;
  size_t size_ = 0;
  T *data_ = nullptr;

 public:

  Vector() {
  }

  explicit Vector(size_t size) { 
	data_ = static_cast<T *>(operator new(size * sizeof(T)));
    size_ = size;
    capacity_ = size;
    for (size_t i = 0; i < size; ++i) {
      new (data_ + i) T();
    }
  }

  Vector(const Vector<T> &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = static_cast<T *>(operator new(capacity_ * sizeof(T)));
    for (size_t i = 0; i < size_; ++i) {
      new (data_ + i) T(other.data_[i]);
    }
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  void Clear() {
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    size_ = 0;
  }

  bool Empty() const { 
    return !size_;
  }

  void PushBack(const T &el) { 
    if (size_ + 1 > capacity_) {
      size_t new_cap = (capacity_ != 0) ? 2 * capacity_ : 1;
      T *temp = static_cast<T *>(operator new(sizeof(T) * new_cap));
      for (size_t i = 0; i < size_; ++i) {
        new (temp + i) T(std::move(data_[i]));
      }
      new (temp + size_) T(el);
      for (size_t i = 0; i < size_; ++i) {  // Тут беды возможно
        (data_ + i)->~T();
      }
      capacity_ = new_cap;
      size_ += 1;
      operator delete(data_);
      data_ = temp;
    } else {
      new (data_ + size_) T(el);
      size_ += 1;
    }
  }

  void PopBack() { 
    assert(size_ > 0);
    size_ -= 1;
    (data_ + size_)->~T();
  }

  void Resize(size_t count) {
    if (count == size_) {
      return;
    }
    if (count < size_) {
      size_t copy = size_;
      for (size_t i = count; i < copy; ++i) {
        (data_ + i)->~T();
        --size_;
      }
      return;
    }
    for (size_t i = size_; i < count; ++i) {
      PushBack(T());
    }
  }

  void ShrinkToFit() {
    if (size_ != capacity_) {
      T *temp = static_cast<T *>(operator new(sizeof(T) * size_));
      for (size_t i = 0; i < size_; ++i) {
        new (temp + i) T(std::move(data_[i]));
      }
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      capacity_ = size_;
      data_ = temp;
    }
  }

  T& operator[](size_t index) { 
    assert(index < size_);
    return *(data_ + index);
  }

  ~Vector() {
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    operator delete(data_);
  }
};


#endif
