#include <iostream>
#include <limits>

#include "custom_int.h"

using namespace std;

typedef Int<13> Int13;
typedef UInt<5> Uint5;

int main() {
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
}
