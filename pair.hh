#pragma once
#include "combinator.hh"
namespace pair {
/**
 * Pair = λabf.f a b = V
 */
inline const lambda Pair{[](lambda first) {
  return lambda{[first](lambda second) {
    return lambda::pair(combinator::V * first * second);
  }};
}};

/**
 * Fst = λp.p K = Th K
 */
inline const lambda Fst = combinator::Th * combinator::K;
/**
 * Snd = λp.p KI = Th KI
 */
inline const lambda Snd = combinator::Th * combinator::KI;

} // namespace pair

inline std::string print_pair(const lambda &pair) {
  std::string result;
  const auto first = pair::Fst(pair);
  const auto second = pair::Snd(pair);
  return toString(first) + "," + toString(second);
}

inline std::string lambda::print_pair(const lambda &pair) const {
  return ::print_pair(pair);
}