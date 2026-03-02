#include <bitset>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "custom_int.h"

using namespace std;

typedef Int<13> Int13;
typedef UInt<5> Uint5;

template<size_t Size>
std::bitset<Size> TwosComplementBits(std::bitset<Size> value) {
  value.flip();
  bool carry = true;
  for (size_t i = 0; i < Size && carry; ++i) {
    const bool bit = value[i];
    value[i] = !bit;
    carry = bit;
  }
  return value;
}

void RunDivisionTests() {
  using UInt130 = UInt<130>;
  using Int130 = Int<130>;
  using UInt3 = UInt<3>;
  using UInt5 = UInt<5>;
  using Int5 = Int<5>;

  std::bitset<130> dividend_bits;
  dividend_bits.set(120);
  dividend_bits.set(80);
  dividend_bits.set(12);

  std::bitset<130> divisor_bits;
  divisor_bits.set(80);

  UInt130 dividend;
  dividend = dividend_bits;
  UInt130 divisor;
  divisor = divisor_bits;

  UInt130 quotient = dividend;
  quotient /= divisor;

  std::bitset<130> expected_quotient_bits;
  expected_quotient_bits.set(40);
  expected_quotient_bits.set(0);
  UInt130 expected_quotient;
  expected_quotient = expected_quotient_bits;
  assert(quotient == expected_quotient);

  UInt130 product = quotient;
  product *= divisor;

  UInt130 remainder = dividend;
  remainder -= product;

  std::bitset<130> expected_remainder_bits;
  expected_remainder_bits.set(12);
  UInt130 expected_remainder;
  expected_remainder = expected_remainder_bits;
  assert(remainder == expected_remainder);
  assert(remainder < divisor);

  UInt130 reconstructed = product;
  reconstructed += remainder;
  assert(reconstructed == dividend);

  std::bitset<130> positive_power_100;
  positive_power_100.set(100);
  std::bitset<130> negative_power_100 = TwosComplementBits(positive_power_100);

  std::bitset<130> positive_power_5;
  positive_power_5.set(5);
  std::bitset<130> negative_power_5 = TwosComplementBits(positive_power_5);

  Int130 signed_dividend;
  signed_dividend = negative_power_100;
  Int130 signed_divisor;
  signed_divisor = positive_power_5;

  Int130 signed_quotient = signed_dividend;
  signed_quotient /= signed_divisor;

  Int130 signed_reconstructed = signed_quotient;
  signed_reconstructed *= signed_divisor;
  assert(signed_reconstructed == signed_dividend);

  Int130 negative_divisor;
  negative_divisor = negative_power_5;
  Int130 positive_quotient = signed_dividend;
  positive_quotient /= negative_divisor;

  std::bitset<130> expected_positive_quotient_bits;
  expected_positive_quotient_bits.set(95);
  Int130 expected_positive_quotient;
  expected_positive_quotient = expected_positive_quotient_bits;
  assert(positive_quotient == expected_positive_quotient);

  UInt3 u3 = 7;
  u3 /= 2;
  assert(u3.to_int() == 3);

  UInt5 u5 = 31;
  u5 /= 3;
  assert(u5.to_int() == 10);

  Int5 s5 = -7;
  s5 /= 2;
  assert(s5.to_int() == -3);

  Int5 s5_neg_divisor = -7;
  s5_neg_divisor /= -2;
  assert(s5_neg_divisor.to_int() == 3);

  Int5 min5 = -16;
  min5 /= -1;
  assert(min5.to_int() == -16);

  bool threw = false;
  try {
    UInt130 divide_by_zero = dividend;
    UInt130 zero;
    divide_by_zero /= zero;
  }
  catch (const std::domain_error &) {
    threw = true;
  }
  assert(threw);

  threw = false;
  try {
    Int130 divide_by_zero = signed_dividend;
    Int130 zero;
    divide_by_zero /= zero;
  }
  catch (const std::domain_error &) {
    threw = true;
  }
  assert(threw);

  threw = false;
  try {
    Int13 small = 10;
    small /= 0;
  }
  catch (const std::domain_error &) {
    threw = true;
  }
  assert(threw);

  threw = false;
  try {
    UInt3 zero_u3 = 1;
    zero_u3 /= 0;
  }
  catch (const std::domain_error &) {
    threw = true;
  }
  assert(threw);
}

void RunBenchmarks() {
  using namespace std::chrono;

  constexpr int kSmallIterations = 5000000;
  constexpr int kWideIterations = 50000;

  volatile unsigned long long small_sink = 0;
  volatile unsigned long long wide_sink = 0;

  UInt<5> small_u = 29;
  UInt<5> small_divisor = 3;

  auto small_start = steady_clock::now();
  for (int i = 0; i < kSmallIterations; ++i) {
    small_u += 7;
    small_u *= 5;
    small_u /= small_divisor;
    small_sink += small_u.to_int();
  }
  auto small_end = steady_clock::now();

  std::bitset<130> wide_a_bits;
  wide_a_bits.set(120);
  wide_a_bits.set(64);
  wide_a_bits.set(12);

  std::bitset<130> wide_b_bits;
  wide_b_bits.set(63);
  wide_b_bits.set(5);

  UInt<130> wide_a;
  wide_a = wide_a_bits;
  UInt<130> wide_b;
  wide_b = wide_b_bits;

  auto wide_start = steady_clock::now();
  for (int i = 0; i < kWideIterations; ++i) {
    UInt<130> q = wide_a;
    q /= wide_b;
    wide_a -= q;
    wide_a += wide_b;
    wide_sink += q.data()[0];
  }
  auto wide_end = steady_clock::now();

  const auto small_ms = duration_cast<milliseconds>(small_end - small_start).count();
  const auto wide_ms = duration_cast<milliseconds>(wide_end - wide_start).count();

  cout << "Benchmark (UInt<5>, " << kSmallIterations << " iters): " << small_ms << " ms" << endl;
  cout << "Benchmark (UInt<130>, " << kWideIterations << " iters): " << wide_ms << " ms" << endl;
  cout << "Benchmark sinks: " << small_sink << ", " << wide_sink << endl;
}

int main(int argc, char **argv) {
  Int13 test1 = 1;
  Uint5 test2;
  Int<64> large_signed;
  UInt<64> large_unsigned;

  cout << "test1: " << test1 << endl;
  cout << "test2: " << test2 << endl;

  test1++;
  cout << "test1: " << test1 << endl;

  test1--;
  cout << "test1: " << test1 << endl;

  test1 -= 10;
  cout << "test1: " << test1 << endl;

  auto test3 = test1;

  cout << "test3: " << test3 << endl;

  test3 /= 3;

  cout << "test3: " << test3 << endl;

  test2 += 20;

  cout << "test2: " << test2 << endl;

  test2 = test2 / 10;

  cout << "test2: " << test2 << endl;

  large_signed = numeric_limits<long long>::max();

  cout << "large_signed: " << large_signed << endl;

  large_signed = numeric_limits<long long>::min();

  cout << "large_signed: " << large_signed << endl;

  large_unsigned = numeric_limits<unsigned long long>::max();

  cout << "large_unsigned: " << large_unsigned << endl;

  large_unsigned -= 100000;

  cout << "large_unsigned: " << large_unsigned << endl;

  auto another_large = large_signed;

  another_large -= 100000;

  cout << large_signed << " < " << another_large << "? ";
  large_signed < another_large ? (cout << "Yes" << endl) : (cout << "No" << endl);

  another_large = large_signed;

  cout << "another_large: " << another_large << endl;

  cout << large_signed << " <= " << another_large << "? ";
  large_signed <= another_large ? (cout << "Yes" << endl) : (cout << "No" << endl);

  cout << large_signed << " < " << another_large << "? ";
  large_signed < another_large ? (cout << "Yes" << endl) : (cout << "No" << endl);

  cout << large_signed << " > " << another_large << "? ";
  large_signed > another_large ? (cout << "Yes" << endl) : (cout << "No" << endl);

  cout << large_signed << " >= " << another_large << "? ";
  large_signed >= another_large ? (cout << "Yes" << endl) : (cout << "No" << endl);

  RunDivisionTests();
  cout << "Division tests passed" << endl;

  if (argc > 1 && std::string(argv[1]) == "--benchmark") {
    RunBenchmarks();
  }
}
