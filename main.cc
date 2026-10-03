#include "boolean.hh"
#include "list.hh"
#include <iostream>

int main() {
  auto const l = list::Take(10) *= list::Withindex *= list::Repeat *=
      boolean::False;
  std::cout << l << std::endl;
  return 0;
}