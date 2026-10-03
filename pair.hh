#pragma once
#include "combinator.hh"
namespace pair {
/**
 * Pair = λabf.f a b = V
 */
inline const lambda Pair = combinator::V;

/**
 * Fst = λp.p K = Th K
 */
inline const lambda Fst = combinator::Th * combinator::K;
/**
 * Snd = λp.p KI = Th KI
 */
inline const lambda Snd = combinator::Th * combinator::KI;

} // namespace pair