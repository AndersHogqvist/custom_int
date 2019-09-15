/*
 * int_types.h
 *
 *  Created on: Sep 1, 2019
 *      Author: Anders Högqvist
 */

#ifndef CUSTOM_INT_H_
#define CUSTOM_INT_H_

#include <bitset>
#include <type_traits>

#include <iostream>

const static size_t kSizeThreshold = 64;

template<size_t Size>
class TypeBase {
public:
  size_t size() const {
    return data_.size();
  }

  unsigned long to_ulong() const {
    return data_.to_ulong();
  }

  unsigned long long to_ullong() const {
    return data_.to_ullong();
  }

  std::bitset<Size> data() const {
    return data_;
  }

  std::string to_string() const {
    return data_.to_string();
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator=(const T value) {
    data_ = std::bitset<Size>(value);
    return *this;
  }

  bool operator==(const TypeBase<Size> &other) const {
    return data_ == other.data();
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator==(const T value) const {
    return data_.to_ullong() == value;
  }

  bool operator!=(const TypeBase<Size> &other) const {
    return !(*this == other);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator!=(const T value) const {
    return data_ != std::bitset<Size>(value);
  }

  bool operator<(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return false;
    }
    if (Size <= kSizeThreshold) {
      return to_ullong() < other.to_ullong();
    }
    return less_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_ullong() < value;
    }
    return less_than_(std::bitset<Size>(value));
  }

  bool operator<=(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return true;
    }
    if (Size <= kSizeThreshold) {
      return to_ullong() <= other.to_ullong();
    }
    return less_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<=(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_ullong() <= value;
    }
    return less_than_or_eq_(std::bitset<Size>(value));
  }

  bool operator>(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return false;
    }
    if (Size <= kSizeThreshold) {
      return to_ullong() > other.to_ullong();
    }
    return greater_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_ullong() > value;
    }
    return greater_than_(std::bitset<Size>(value));
  }

  bool operator>=(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return true;
    }
    if (Size <= kSizeThreshold) {
      return to_ullong() >= other.to_ullong();
    }
    return greater_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>=(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_ullong() >= value;
    }
    return greater_than_or_eq_(std::bitset<Size>(value));
  }

  TypeBase<Size> &operator+(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  TypeBase<Size> &operator+(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator+(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + value;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(value);
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  TypeBase<Size> &operator+=(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  TypeBase<Size> &operator+=(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator+=(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = to_ullong() + value;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(value);
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other[ix], carry);
      }
    }
    return *this;
  }

  TypeBase<Size> &operator++(int) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() + 1;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(1);
      for (size_t ix = 0; ix < Size; ++ix) {
        data_[ix] = add_(data_[ix], other[ix], carry);
      }
    }
    return *this;
  }

  TypeBase<Size> &operator-(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - other.to_ullong();
    }
    else {
      subtract_(other.data_);
    }
    return *this;
  }

  TypeBase<Size> &operator-(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - other.to_ullong();
    }
    else {
      subtract_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator-(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - value;
    }
    else {
      subtract_(std::bitset<Size>(value));
    }
    return *this;
  }

  TypeBase<Size> &operator-=(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - other.to_ullong();
    }
    else {
      subtract_(other.data_);
    }
    return *this;
  }

  TypeBase<Size> &operator-=(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - other.to_ullong();
    }
    else {
      subtract_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator-=(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - value;
    }
    else {
      subtract_(std::bitset<Size>(value));
    }
    return *this;
  }

  TypeBase<Size> &operator--(int) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() - 1;
    }
    else {
      subtract_(std::bitset<Size>(1));
    }
    return *this;
  }

  TypeBase<Size> &operator*(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() * other.to_ullong();
    }
    else {
      multiply_(other.data_);
    }
    return *this;
  }

  TypeBase<Size> &operator*(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() * other.to_ullong();
    }
    else {
      multiply_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator*(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() * value;
    }
    else {
      multiply_(std::bitset<Size>(value));
    }
    return *this;
  }

  TypeBase<Size> &operator*=(const TypeBase<Size> &other) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() * other.to_ullong();
    }
    else {
      multiply_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator*=(const T value) {
    if (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() * value;
    }
    else {
      multiply_(std::bitset<Size>(value));
    }
    return *this;
  }

  TypeBase<Size> &operator/(const TypeBase<Size> &other) {
    data_ = data_.to_ullong() / other.to_ullong();
    return *this;
  }

  TypeBase<Size> &operator/(const std::bitset<Size> &other) {
    data_ = data_.to_ullong() / other.to_ullong();
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator/(const T value) {
    data_ = data_.to_ullong() / value;
    return *this;
  }

  TypeBase<Size> &operator/=(const TypeBase<Size> &other) {
    data_ = to_ullong() / other.to_ullong();
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator/=(const T value) {
    data_ = to_ullong() / value;
    return *this;
  }

  template<size_t S>
  friend std::ostream &operator <<(std::ostream &out, const TypeBase<S> &u);
protected:
  TypeBase() = default;
  virtual ~TypeBase() = default;

  std::bitset<Size> data_;

  bool add_(bool b1, bool b2, bool &carry) {
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
  }

  void subtract_(const std::bitset<Size> &other) {
    bool borrow = false;
    for (int i = 0; i < Size; i++) {
      if (borrow) {
        if (data_[i]) {
          data_[i] = other[i];
          borrow = other[i];
        }
        else {
          data_[i] = !other[i];
          borrow = true;
        }
      }
      else {
        if (data_[i]) {
          data_[i] = !other[i];
          borrow = false;
        }
        else {
          data_[i] = other[i];
          borrow = other[i];
        }
      }
    }
  }

  void multiply_(const std::bitset<Size> &other) {
    std::bitset<Size> tmp = data_;
    data_.reset();
    if (tmp.count() < other.count()) {
      for (int i = 0; i < Size; i++) {
        if (tmp[i]) {
          operator+=(other << i);
        }
      }
    }
    else {
      for (int i = 0; i < Size; i++) {
        if (other[i]) {
          operator+=(tmp << i);
        }
      }
    }
  }

  bool less_than_or_eq_(const std::bitset<Size> &other) const {
    for (int i = Size - 1; i >= 0; i--) {
      if (data_[i] && !other[i]) {
        return false;
      }
      if (!data_[i] && other[i]) {
        return true;
      }
    }
    return true;
  }

  bool less_than_(const std::bitset<Size> &other) const {
    for (int i = Size - 1; i >= 0; i--) {
      if (data_[i] && !other[i]) {
        return false;
      }
      if (!data_[i] && other[i]) {
        return true;
      }
    }
    return false;
  }

  bool greater_than_or_eq_(const std::bitset<Size> &other) const {
    for (int i = Size - 1; i >= 0; i--) {
      if (data_[i] && !other[i]) {
        return true;
      }
      if (!data_[i] && other[i]) {
        return false;
      }
    }
    return true;
  }

  bool greater_than_(const std::bitset<Size> &other) const {
    for (int i = Size - 1; i >= 0; i--) {
      if (data_[i] && !other[i]) {
        return true;
      }
      if (!data_[i] && other[i]) {
        return false;
      }
    }
    return false;
  }
};

template<size_t Size>
class UInt : public TypeBase<Size> {
public:
  UInt() = default;

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  UInt(const T value) {
    TypeBase<Size>::operator=(value);
  }

  ~UInt() = default;

  UInt<Size> &operator=(const TypeBase<Size> &other) {
    if (this->data_ == other.data()) {
      return *this;
    }
    this->data_ = other.data();
    return *this;
  }

  UInt<Size> &operator=(const std::bitset<Size> &other) {
    if (this->data_ == other) {
      return *this;
    }
    this->data_ = other;
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  UInt<Size> &operator=(const T value) {
    TypeBase<Size>::operator=(value);
    return *this;
  }

  unsigned long long to_int() const {
    return this->data_.to_ullong();
  }
};

template<size_t Size>
std::ostream &operator<<(std::ostream &out, const UInt<Size> &u) {
  out << u.to_ullong();
  return out;
}

template<size_t Size>
class Int : public TypeBase<Size> {
public:
  Int() = default;

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int(const T value) {
    TypeBase<Size>::operator=(value);
  }

  ~Int() = default;

  Int<Size> &operator=(const TypeBase<Size> &other) {
    if (this->data_ == other.data()) {
      return *this;
    }
    this->data_ = other.data();
    return *this;
  }

  Int<Size> &operator=(const std::bitset<Size> &other) {
    if (this->data_ == other) {
      return *this;
    }
    this->data_ = other;
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator=(const T value) {
    TypeBase<Size>::operator=(value);
    return *this;
  }

  long long to_int() const {
    if (this->data_[Size - 1]) {
      std::bitset<Size> tmp = this->data_;
      tmp.flip();
      return tmp.to_ullong() * -1 - 1;
    }
    return this->data_.to_ullong();
  }

  bool operator<(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return false;
    }
    if (Size <= kSizeThreshold) {
      return to_int() < other.to_int();
    }
    return TypeBase<Size>::less_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_int() < value;
    }
    return TypeBase<Size>::less_than_(std::bitset<Size>(value));
  }

  bool operator<=(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return true;
    }
    if (Size <= kSizeThreshold) {
      return to_int() <= other.to_int();
    }
    return TypeBase<Size>::less_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<=(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_int() <= value;
    }
    return TypeBase<Size>::less_than_or_eq_(std::bitset<Size>(value));
  }

  bool operator>(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return false;
    }
    if (Size <= kSizeThreshold) {
      return to_int() > other.to_int();
    }
    return TypeBase<Size>::greater_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_int() > value;
    }
    return TypeBase<Size>::greater_than_(std::bitset<Size>(value));
  }

  bool operator>=(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return true;
    }
    if (Size <= kSizeThreshold) {
      return to_int() >= other.to_int();
    }
    return TypeBase<Size>::greater_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>=(const T value) const {
    if (Size <= kSizeThreshold) {
      return to_int() >= value;
    }
    return TypeBase<Size>::greater_than_or_eq_(std::bitset<Size>(value));
  }

  Int<Size> &operator+(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + other.to_int();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  Int<Size> &operator+(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator+(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + value;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(value);
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  Int<Size> &operator+=(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + other.to_int();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  Int<Size> &operator+=(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + other.to_ullong();
    }
    else {
      bool carry = false;
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other.data_[ix], carry);
      }
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator+=(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + value;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(value);
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = add_(this->data_[ix], other[ix], carry);
      }
    }
    return *this;
  }

  Int<Size> &operator++(int) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() + 1;
    }
    else {
      bool carry = false;
      std::bitset<Size> other(1);
      for (size_t ix = 0; ix < Size; ++ix) {
        this->data_[ix] = TypeBase<Size>::add_(this->data_[ix], other[ix], carry);
      }
    }
    return *this;
  }

  Int<Size> &operator-(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - other.to_int();
    }
    else {
      TypeBase<Size>::subtract_(other.data_);
    }
    return *this;
  }

  Int<Size> &operator-(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - other.to_ullong();
    }
    else {
      TypeBase<Size>::subtract_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator-(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - value;
    }
    else {
      TypeBase<Size>::subtract_(std::bitset<Size>(value));
    }
    return *this;
  }

  Int<Size> &operator-=(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - other.to_int();
    }
    else {
      TypeBase<Size>::subtract_(other.data_);
    }
    return *this;
  }

  Int<Size> &operator-=(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - other.to_ullong();
    }
    else {
      TypeBase<Size>::subtract_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator-=(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - value;
    }
    else {
      TypeBase<Size>::subtract_(std::bitset<Size>(value));
    }
    return *this;
  }

  Int<Size> &operator--(int) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() - 1;
    }
    else {
      TypeBase<Size>::subtract_(std::bitset<Size>(1));
    }
    return *this;
  }

  Int<Size> &operator*(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() * other.to_int();
    }
    else {
      TypeBase<Size>::multiply_(other.data_);
    }
    return *this;
  }

  Int<Size> &operator*(const std::bitset<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() * other.to_ullong();
    }
    else {
      TypeBase<Size>::multiply_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator*(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() * value;
    }
    else {
      TypeBase<Size>::multiply_(std::bitset<Size>(value));
    }
    return *this;
  }

  Int<Size> &operator*=(const Int<Size> &other) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() * other.to_int();
    }
    else {
      TypeBase<Size>::multiply_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator*=(const T value) {
    if (Size <= kSizeThreshold) {
      this->data_ = to_int() * value;
    }
    else {
      TypeBase<Size>::multiply_(std::bitset<Size>(value));
    }
    return *this;
  }

  Int<Size> &operator/(const Int<Size> &other) {
    this->data_ = to_int() / other.to_int();
    return *this;
  }

  Int<Size> &operator/(const std::bitset<Size> &other) {
    this->data_ = to_int() / other.to_ullong();
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator/(const T value) {
    this->data_ = to_int() / value;
    return *this;
  }

  Int<Size> &operator/=(const Int<Size> &other) {
    this->data_ = to_int() / other.to_int();
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator/=(const T value) {
    this->data_ = to_int() / value;
    return *this;
  }
};

template<size_t Size>
std::ostream &operator<<(std::ostream &out, const Int<Size> &u) {
  out << u.to_int();
  return out;
}

#endif /* CUSTOM_INT_H_ */
