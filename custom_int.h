/*
 * int_types.h
 *
 *  Created on: Sep 1, 2019
 *      Author: Anders Högqvist
 */

#ifndef CUSTOM_INT_H_
#define CUSTOM_INT_H_

#include <bitset>
#include <stdexcept>
#include <type_traits>

#include <iostream>

static constexpr size_t kSizeThreshold = 64;

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

  const std::bitset<Size> &data() const {
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
    return data_ == other.data_;
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
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() < other.to_ullong();
    }
    return less_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() < value;
    }
    return less_than_(std::bitset<Size>(value));
  }

  bool operator<=(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return true;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() <= other.to_ullong();
    }
    return less_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<=(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() <= value;
    }
    return less_than_or_eq_(std::bitset<Size>(value));
  }

  bool operator>(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return false;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() > other.to_ullong();
    }
    return greater_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() > value;
    }
    return greater_than_(std::bitset<Size>(value));
  }

  bool operator>=(const TypeBase<Size> &other) const {
    if (data_ == other.data_) {
      return true;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_ullong() >= other.to_ullong();
    }
    return greater_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>=(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
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
        data_[ix] = add_(data_[ix], other[ix], carry);
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
        data_[ix] = add_(data_[ix], other[ix], carry);
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
        data_[ix] = add_(data_[ix], other[ix], carry);
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
    if (other.data_.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() / other.to_ullong();
    }
    else {
      data_ = divide_unsigned_(data_, other.data_);
    }
    return *this;
  }

  TypeBase<Size> &operator/(const std::bitset<Size> &other) {
    if (other.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() / other.to_ullong();
    }
    else {
      data_ = divide_unsigned_(data_, other);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator/(const T value) {
    std::bitset<Size> other(value);
    if (other.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      data_ = data_.to_ullong() / value;
    }
    else {
      data_ = divide_unsigned_(data_, other);
    }
    return *this;
  }

  TypeBase<Size> &operator/=(const TypeBase<Size> &other) {
    if (other.data_.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      data_ = to_ullong() / other.to_ullong();
    }
    else {
      data_ = divide_unsigned_(data_, other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  TypeBase<Size> &operator/=(const T value) {
    std::bitset<Size> other(value);
    if (other.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      data_ = to_ullong() / value;
    }
    else {
      data_ = divide_unsigned_(data_, other);
    }
    return *this;
  }

  template<size_t S>
  friend std::ostream &operator <<(std::ostream &out, const TypeBase<S> &u);
protected:
  TypeBase() = default;
  ~TypeBase() = default;

  std::bitset<Size> data_;

  bool add_(bool b1, bool b2, bool &carry) {
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
  }

  void subtract_(const std::bitset<Size> &other) {
    bool borrow = false;
    for (size_t i = 0; i < Size; ++i) {
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
      for (size_t i = 0; i < Size; ++i) {
        if (tmp[i]) {
          operator+=(other << i);
        }
      }
    }
    else {
      for (size_t i = 0; i < Size; ++i) {
        if (other[i]) {
          operator+=(tmp << i);
        }
      }
    }
  }

  bool less_than_or_eq_(const std::bitset<Size> &other) const {
    for (size_t i = Size; i-- > 0;) {
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
    for (size_t i = Size; i-- > 0;) {
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
    for (size_t i = Size; i-- > 0;) {
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
    for (size_t i = Size; i-- > 0;) {
      if (data_[i] && !other[i]) {
        return true;
      }
      if (!data_[i] && other[i]) {
        return false;
      }
    }
    return false;
  }

  static bool less_than_bits_(const std::bitset<Size> &lhs,
                              const std::bitset<Size> &rhs) {
    for (size_t i = Size; i-- > 0;) {
      if (lhs[i] != rhs[i]) {
        return !lhs[i] && rhs[i];
      }
    }
    return false;
  }

  static void subtract_bits_(std::bitset<Size> &lhs,
                             const std::bitset<Size> &rhs) {
    bool borrow = false;
    for (size_t i = 0; i < Size; ++i) {
      const bool lhs_bit = lhs[i];
      const bool rhs_bit = rhs[i];
      lhs[i] = lhs_bit ^ rhs_bit ^ borrow;
      borrow = (!lhs_bit && (rhs_bit || borrow)) || (rhs_bit && borrow);
    }
  }

  static size_t highest_set_bit_(const std::bitset<Size> &value) {
    for (size_t i = Size; i-- > 0;) {
      if (value[i]) {
        return i;
      }
    }
    return Size;
  }

  static std::bitset<Size> divide_unsigned_(const std::bitset<Size> &dividend,
                                            const std::bitset<Size> &divisor) {
    if (divisor.none()) {
      throw std::domain_error("Division by zero");
    }

    if (dividend.none() || less_than_bits_(dividend, divisor)) {
      return std::bitset<Size>();
    }

    const size_t dividend_msb = highest_set_bit_(dividend);
    const size_t divisor_msb = highest_set_bit_(divisor);
    const size_t shift = dividend_msb - divisor_msb;

    std::bitset<Size> quotient;
    std::bitset<Size> remainder = dividend;
    std::bitset<Size> shifted_divisor = divisor << shift;

    for (size_t current_shift = shift + 1; current_shift-- > 0;) {
      if (!less_than_bits_(remainder, shifted_divisor)) {
        subtract_bits_(remainder, shifted_divisor);
        quotient.set(current_shift);
        if (remainder.none()) {
          break;
        }
      }

      if (current_shift == 0) {
        break;
      }
      shifted_divisor >>= 1;
    }

    return quotient;
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
    this->data_ = other.data();
    return *this;
  }

  UInt<Size> &operator=(const std::bitset<Size> &other) {
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
    this->data_ = other.data();
    return *this;
  }

  Int<Size> &operator=(const std::bitset<Size> &other) {
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
    if constexpr (Size <= kSizeThreshold) {
      return to_int() < other.to_int();
    }
    return TypeBase<Size>::less_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_int() < value;
    }
    return TypeBase<Size>::less_than_(std::bitset<Size>(value));
  }

  bool operator<=(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return true;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_int() <= other.to_int();
    }
    return TypeBase<Size>::less_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator<=(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_int() <= value;
    }
    return TypeBase<Size>::less_than_or_eq_(std::bitset<Size>(value));
  }

  bool operator>(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return false;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_int() > other.to_int();
    }
    return TypeBase<Size>::greater_than_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
      return to_int() > value;
    }
    return TypeBase<Size>::greater_than_(std::bitset<Size>(value));
  }

  bool operator>=(const Int<Size> &other) const {
    if (this->data_ == other.data_) {
      return true;
    }
    if constexpr (Size <= kSizeThreshold) {
      return to_int() >= other.to_int();
    }
    return TypeBase<Size>::greater_than_or_eq_(other.data_);
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  bool operator>=(const T value) const {
    if constexpr (Size <= kSizeThreshold) {
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
        this->data_[ix] = add_(this->data_[ix], other[ix], carry);
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
        this->data_[ix] = add_(this->data_[ix], other[ix], carry);
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
        this->data_[ix] = add_(this->data_[ix], other[ix], carry);
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
    if (other.data_.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      this->data_ = to_int() / other.to_int();
    }
    else {
      divide_signed_(other.data_);
    }
    return *this;
  }

  Int<Size> &operator/(const std::bitset<Size> &other) {
    if (other.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      this->data_ = to_int() / other.to_ullong();
    }
    else {
      divide_signed_(other);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator/(const T value) {
    std::bitset<Size> divisor(value);
    if (divisor.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      this->data_ = to_int() / value;
    }
    else {
      divide_signed_(divisor);
    }
    return *this;
  }

  Int<Size> &operator/=(const Int<Size> &other) {
    if (other.data_.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      this->data_ = to_int() / other.to_int();
    }
    else {
      divide_signed_(other.data_);
    }
    return *this;
  }

  template<typename T,
      typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
  Int<Size> &operator/=(const T value) {
    std::bitset<Size> divisor(value);
    if (divisor.none()) {
      throw std::domain_error("Division by zero");
    }
    if constexpr (Size <= kSizeThreshold) {
      this->data_ = to_int() / value;
    }
    else {
      divide_signed_(divisor);
    }
    return *this;
  }

private:
  static void increment_bits_(std::bitset<Size> &value) {
    bool carry = true;
    for (size_t i = 0; i < Size && carry; ++i) {
      const bool bit = value[i];
      value[i] = !bit;
      carry = bit;
    }
  }

  static std::bitset<Size> twos_complement_(std::bitset<Size> value) {
    value.flip();
    increment_bits_(value);
    return value;
  }

  static std::bitset<Size> magnitude_bits_(const std::bitset<Size> &value) {
    if (!value[Size - 1]) {
      return value;
    }
    return twos_complement_(value);
  }

  void divide_signed_(const std::bitset<Size> &divisor) {
    const bool lhs_negative = this->data_[Size - 1];
    const bool rhs_negative = divisor[Size - 1];

    const std::bitset<Size> lhs_mag = magnitude_bits_(this->data_);
    const std::bitset<Size> rhs_mag = magnitude_bits_(divisor);
    std::bitset<Size> quotient = TypeBase<Size>::divide_unsigned_(lhs_mag, rhs_mag);

    if (lhs_negative ^ rhs_negative) {
      quotient = twos_complement_(quotient);
    }

    this->data_ = quotient;
  }
};

template<size_t Size>
std::ostream &operator<<(std::ostream &out, const Int<Size> &u) {
  out << u.to_int();
  return out;
}

#endif /* CUSTOM_INT_H_ */
