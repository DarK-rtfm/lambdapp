#include "boolean.hh"
#include "list.hh"
#include "number.hh"

#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

namespace {
using BoolBinary = std::function<lambda(bool, bool)>;
using IntUnary = std::function<lambda(int)>;
using IntBinary = std::function<lambda(int, int)>;
using BoolComparison = std::function<lambda(int, int)>;

int failures = 0;

void fail(const std::string &message) {
  ++failures;
  std::cout << "FAIL: " << message << '\n';
}

void check(const std::string &label, bool actual, bool expected) {
  if (actual != expected) {
    fail(label + " expected " + (expected ? "true" : "false") + ", got " +
         (actual ? "true" : "false"));
  }
}

void check(const std::string &label, int actual, int expected) {
  if (actual != expected) {
    fail(label + " expected " + std::to_string(expected) + ", got " +
         std::to_string(actual));
  }
}

void check(const std::string &label, const std::string &actual,
           const std::string &expected) {
  if (actual != expected) {
    fail(label + " expected " + expected + ", got " + actual);
  }
}

void checkBooleanResult(const std::string &label, const lambda &actual,
                        bool expected) {
  check(label + " value", boolean::toBool(actual), expected);
  check(label + " marker", toString(actual), expected ? "True" : "False");
}

void checkIntegerResult(const std::string &label, const lambda &actual,
                        int expected) {
  check(label + " value", number::toInt(actual), expected);
  check(label + " marker", toString(actual), std::to_string(expected));
}

int integerPower(int base, int exponent) {
  int result = 1;
  for (int index = 0; index < exponent; ++index) {
    result *= base;
  }
  return result;
}

void testBooleans() {
  std::cout << "Running Boolean tests...\n";
  const std::array<std::pair<const char *, BoolBinary>, 6> operations{{
      {"And",
       [](bool a, bool b) {
         return boolean::And(boolean::fromBool(a))(boolean::fromBool(b));
       }},
      {"Or",
       [](bool a, bool b) {
         return boolean::Or(boolean::fromBool(a))(boolean::fromBool(b));
       }},
      {"Xor",
       [](bool a, bool b) {
         return boolean::Xor(boolean::fromBool(a))(boolean::fromBool(b));
       }},
      {"Xnor",
       [](bool a, bool b) {
         return boolean::Xnor(boolean::fromBool(a))(boolean::fromBool(b));
       }},
      {"Nand",
       [](bool a, bool b) {
         return boolean::Nand(boolean::fromBool(a))(boolean::fromBool(b));
       }},
      {"Nor",
       [](bool a, bool b) {
         return boolean::Nor(boolean::fromBool(a))(boolean::fromBool(b));
       }},
  }};

  for (bool a : {false, true}) {
    for (bool b : {false, true}) {
      for (const auto &[name, operation] : operations) {
        bool expected = false;
        if (std::string(name) == "And") {
          expected = a && b;
        } else if (std::string(name) == "Or") {
          expected = a || b;
        } else if (std::string(name) == "Xor") {
          expected = a != b;
        } else if (std::string(name) == "Xnor") {
          expected = a == b;
        } else if (std::string(name) == "Nand") {
          expected = !(a && b);
        } else {
          expected = !(a || b);
        }
        checkBooleanResult("" + std::string(name) + "(" + std::to_string(a) +
                               "," + std::to_string(b) + ")",
                           operation(a, b), expected);
      }
    }
  }

  for (bool value : {false, true}) {
    checkBooleanResult("Not(" + std::to_string(value) + ")",
                       boolean::Not(boolean::fromBool(value)), !value);
  }
}

void testNumbers() {
  std::cout << "Running number tests...\n";
  const std::array<std::pair<const char *, IntUnary>, 2> unary_operations{{
      {"Succ", [](int value) { return number::Succ(number::fromInt(value)); }},
      {"Pred", [](int value) { return number::Pred(number::fromInt(value)); }},
  }};

  for (int value = 0; value < 10; ++value) {
    checkIntegerResult("Church(" + std::to_string(value) + ")",
                       number::fromInt(value), value);
    for (const auto &[name, operation] : unary_operations) {
      const int expected =
          name == std::string("Succ") ? value + 1 : std::max(0, value - 1);
      checkIntegerResult(std::string(name) + "(" + std::to_string(value) + ")",
                         operation(value), expected);
    }
  }

  const std::array<std::pair<const char *, IntBinary>, 4> operations{{
      {"Add",
       [](int a, int b) {
         return number::Add(number::fromInt(a))(number::fromInt(b));
       }},
      {"Mul",
       [](int a, int b) {
         return number::Mul(number::fromInt(a))(number::fromInt(b));
       }},
      {"Pow",
       [](int a, int b) {
         return number::Pow(number::fromInt(a))(number::fromInt(b));
       }},
      {"Sub",
       [](int a, int b) {
         return number::Sub(number::fromInt(a))(number::fromInt(b));
       }},
  }};

  for (int a = 1; a < 5; ++a) {
    for (int b = 1; b < 5; ++b) {
      for (const auto &[name, operation] : operations) {
        int expected = 0;
        if (std::string(name) == "Add") {
          expected = a + b;
        } else if (std::string(name) == "Mul") {
          expected = a * b;
        } else if (std::string(name) == "Pow") {
          expected = integerPower(a, b);
        } else {
          expected = std::max(0, a - b);
        }
        checkIntegerResult(std::string(name) + "(" + std::to_string(a) + "," +
                               std::to_string(b) + ")",
                           operation(a, b), expected);
      }
    }
  }
}

void testComparisons() {
  std::cout << "Running comparison tests...\n";
  const std::array<std::pair<const char *, BoolComparison>, 6> operations{{
      {"Eq",
       [](int a, int b) {
         return number::Eq(number::fromInt(a))(number::fromInt(b));
       }},
      {"Neq",
       [](int a, int b) {
         return number::Neq(number::fromInt(a))(number::fromInt(b));
       }},
      {"Gt",
       [](int a, int b) {
         return number::Gt(number::fromInt(a))(number::fromInt(b));
       }},
      {"Lt",
       [](int a, int b) {
         return number::Lt(number::fromInt(a))(number::fromInt(b));
       }},
      {"Geq",
       [](int a, int b) {
         return number::Geq(number::fromInt(a))(number::fromInt(b));
       }},
      {"Leq",
       [](int a, int b) {
         return number::Leq(number::fromInt(a))(number::fromInt(b));
       }},
  }};

  for (int a = 0; a < 10; ++a) {
    for (int b = 0; b < 10; ++b) {
      for (const auto &[name, operation] : operations) {
        bool expected = false;
        if (std::string(name) == "Eq") {
          expected = a == b;
        } else if (std::string(name) == "Neq") {
          expected = a != b;
        } else if (std::string(name) == "Gt") {
          expected = a > b;
        } else if (std::string(name) == "Lt") {
          expected = a < b;
        } else if (std::string(name) == "Geq") {
          expected = a >= b;
        } else {
          expected = a <= b;
        }
        checkBooleanResult(std::string(name) + "(" + std::to_string(a) + "," +
                               std::to_string(b) + ")",
                           operation(a, b), expected);
      }
    }
  }
}

void testRecursion() {
  std::cout << "Running recursion tests...\n";
  const std::array<int, 5> factorials{{1, 1, 2, 6, 24}};
  for (int value = 0; value < static_cast<int>(factorials.size()); ++value) {
    checkIntegerResult("fac(" + std::to_string(value) + ")",
                       number::fac(number::fromInt(value)), factorials[value]);
  }

  const std::array<int, 10> fibonacci{{1, 1, 2, 3, 5, 8, 13, 21, 34, 55}};
  for (int value = 0; value < static_cast<int>(fibonacci.size()); ++value) {
    checkIntegerResult("fib(" + std::to_string(value) + ")",
                       number::fib(number::fromInt(value)), fibonacci[value]);
  }
}

void testFormatting() {
  std::cout << "Running formatting tests...\n";
  auto true_value = boolean::fromBool(true);
  auto false_value = boolean::fromBool(false);
  check("True marker", toString(boolean::And(true_value)(true_value)), "True");
  check("False marker", toString(boolean::Or(true_value)(false_value)), "True");
  check("Not marker", toString(boolean::Not(false_value)), "True");
  check("Number marker", toString(number::fromInt(42)), "42");
  check("Empty list", print_list(list::Nil), "Nil");

  auto booleans = true_value %= false_value %= list::Nil;
  check("Boolean list", print_list(booleans), "(True) %= (False) %= Nil");
}

void testListLengths() {
  std::cout << "Running list length tests...\n";
  auto booleans = boolean::True %= boolean::False %= list::Nil;
  auto numbers = 1 %= 2 %= 3 %= 4 %= list::Nil;
  auto mixed = boolean::True %= 7 %= boolean::False %= list::Nil;

  const std::array<std::pair<const char *, std::pair<lambda, int>>, 4> cases{{
      {"empty", {list::Nil, 0}},
      {"booleans", {booleans, 2}},
      {"numbers", {numbers, 4}},
      {"mixed", {mixed, 3}},
  }};

  for (const auto &[name, test_case] : cases) {
    std::cout << "  Len(" << name << ")\n";
    checkIntegerResult(std::string("Len(") + name + ")",
                       list::Len(test_case.first), test_case.second);
  }
}

void testListReverse() {
  std::cout << "Running list Reverse tests...\n";
  auto booleans = boolean::True %= boolean::False %= list::Nil;
  auto numbers = 1 %= 2 %= 3 %= 4 %= list::Nil;
  auto mixed = boolean::True %= 7 %= boolean::False %= list::Nil;

  const std::array<std::pair<const char *, std::pair<lambda, const char *>>, 4>
      cases{{
          {"empty", {list::Nil, "Nil"}},
          {"numbers", {numbers, "(4) %= (3) %= (2) %= (1) %= Nil"}},
          {"booleans", {booleans, "(False) %= (True) %= Nil"}},
          {"mixed", {mixed, "(False) %= (7) %= (True) %= Nil"}},
      }};

  for (const auto &[name, test_case] : cases) {
    std::cout << "  Reverse(" << name << ")\n";
    check(std::string("Reverse(") + name + ")",
          print_list(list::Reverse(test_case.first)), test_case.second);
  }
}

void testListRangeAndIota() {
  std::cout << "Running list Range and Iota tests...\n";

  const std::array<std::pair<const char *, std::pair<int, const char *>>, 4>
      iota_cases{{
          {"empty", {0, "Nil"}},
          {"one", {1, "(0) %= Nil"}},
          {"prefix", {4, "(0) %= (1) %= (2) %= (3) %= Nil"}},
          {"longer", {6, "(0) %= (1) %= (2) %= (3) %= (4) %= (5) %= Nil"}},
      }};

  for (const auto &[name, test_case] : iota_cases) {
    std::cout << "  Iota(" << name << ")\n";
    const auto &[count, expected] = test_case;
    check(std::string("Iota(") + name + ")",
          print_list(list::Iota(number::fromInt(count))), expected);
  }

  // struct RangeCase {
  //   const char *name;
  //   int start;
  //   int end;
  //   const char *expected;
  // };
  // const std::array<RangeCase, 4> range_cases{{
  //     {"from-zero", 0, 4, "(0) %= (1) %= (2) %= (3) %= Nil"},
  //     {"middle", 2, 5, "(2) %= (3) %= (4) %= Nil"},
  //     {"equal-bounds", 3, 3, "Nil"},
  //     {"past-end", 5, 3, "Nil"},
  // }};

  // for (const auto &[name, start, end, expected] : range_cases) {
  //   std::cout << "  Range(" << name << ")\n";
  //   check(std::string("Range(") + name + ")",
  //         print_list(list::Range(number::fromInt(start))(number::fromInt(end))),
  //         expected);
  // }
}

// void testListDrop() {
//   std::cout << "Running list Drop tests...\n";
//   auto booleans = boolean::True %= boolean::False %= list::Nil;
//   auto numbers = 1 %= 2 %= 3 %= 4 %= list::Nil;
//   auto mixed = boolean::True %= 7 %= boolean::False %= list::Nil;
//
//   const std::array<
//       std::pair<const char *, std::tuple<int, lambda, const char *>>, 6>
//       cases{{
//           {"zero", {0, numbers, "(1) %= (2) %= (3) %= (4) %= Nil"}},
//           {"empty", {3, list::Nil, "Nil"}},
//           {"prefix", {2, numbers, "(3) %= (4) %= Nil"}},
//           {"exact", {4, numbers, "Nil"}},
//           {"longer", {6, numbers, "Nil"}},
//           {"mixed", {1, mixed, "(7) %= (False) %= Nil"}},
//       }};
//
//   for (const auto &[name, test_case] : cases) {
//     std::cout << "  Drop(" << name << ")\n";
//     const auto &[count, input, expected] = test_case;
//     check(std::string("Drop(") + name + ")",
//           print_list(list::Drop(number::fromInt(count))(input)), expected);
//   }
// }

void testListTake() {
  std::cout << "Running list Take tests...\n";
  auto booleans = boolean::True %= boolean::False %= list::Nil;
  auto numbers = 1 %= 2 %= 3 %= 4 %= list::Nil;
  auto mixed = boolean::True %= 7 %= boolean::False %= list::Nil;

  const std::array<
      std::pair<const char *, std::tuple<int, lambda, const char *>>, 6>
      cases{{
          {"zero", {0, numbers, "Nil"}},
          {"empty", {3, list::Nil, "Nil"}},
          {"prefix", {2, numbers, "(1) %= (2) %= Nil"}},
          {"exact", {4, numbers, "(1) %= (2) %= (3) %= (4) %= Nil"}},
          {"longer", {6, numbers, "(1) %= (2) %= (3) %= (4) %= Nil"}},
          {"mixed", {2, mixed, "(True) %= (7) %= Nil"}},
      }};

  for (const auto &[name, test_case] : cases) {
    std::cout << "  Take(" << name << ")\n";
    const auto &[count, input, expected] = test_case;
    check(std::string("Take(") + name + ")",
          print_list(list::Take(number::fromInt(count))(input)), expected);
  }
}
} // namespace

int main() {
  testBooleans();
  testNumbers();
  testComparisons();
  testRecursion();
  testFormatting();
  testListLengths();
  testListReverse();
  // testListDrop();
  testListTake();
  testListRangeAndIota();

  if (failures != 0) {
    std::cout << failures << " test(s) failed.\n";
  }
  return failures == 0 ? 0 : 1;
}
