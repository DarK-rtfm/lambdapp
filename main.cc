#include "list.hh"
#include <iostream>

int main() {
  std::cout << list::Take * 10 * list::Inflist << std::endl;
  std::cout << list::Sum(list::Iota * 10) << std::endl;
  std::cout << list::Len(list::Iota * 10) << std::endl;
  return 0;
}