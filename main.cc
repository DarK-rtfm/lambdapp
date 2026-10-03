#include "boolean.hh"
#include "list.hh"
#include <iostream>

int main() {
  std::cout << list::Withindex(1 %= list::Nil) << std::endl;
  std::cout << list::Sum(list::Iota * 10) << std::endl;
  std::cout << list::Len(list::Iota * 10) << std::endl;
  return 0;
}