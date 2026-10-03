#include "boolean.hh"
#include "list.hh"
#include <iostream>

int main() {
  auto const l = list::Iota(10);
  std::cout << list::Map * number::Even * l << std::endl;
  std::cout << list::Map * number::Odd * l << std::endl;
  std::cout << l << std::endl;
  return 0;
}