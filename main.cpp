#include <iostream>

#include "custom_int.h"

using namespace std;

int main() {
  Int<13> test1 = 1;
  UInt<5> test2;

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
}
