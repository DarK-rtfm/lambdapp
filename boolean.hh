#pragma once
#include "combinator.hh"

namespace boolean {
/**
 * True = λab.a = K
 */
inline const auto True = lambda::boolean(true, combinator::K);

/**
 * False = λab.b = KI
 */
inline const auto False = lambda::boolean(false, combinator::KI);

// force to bool in case b doesnt originate from a lambda::boolean
inline lambda coherseBool(const lambda &b) {
  const auto result = b(True)(False);
  return result;
}

/**
 * Not = λpab.p b a = C
 */
inline const auto Not = coherseBool |= combinator::C;

/**
 * And = λab.a b a = λab. C a a b = λa . C a a = λa. C a (I a) = λa. S C  I a =
 * S C I
 */
inline const auto And = coherseBool ^=
    combinator::S * combinator::C * combinator::I;

/**
 * Or = λab.a a b = λa . a a = λa. I a (I a) = λa. S I I a = S I I
 */
inline const auto Or = coherseBool ^=
    combinator::S * combinator::I * combinator::I;

/**
 * Xor = λab.a (Not b) b = λab. C a b (Not b) = λab. S (C a) Not b = λa. S (C a)
 * C = λa. C S C (C a) = λa. B (C S C) (C) a = B (C S C) C
 */
inline const auto Xor = coherseBool ^=
    combinator::C *combinator::S *combinator::C |= combinator::C;

/**
 * Xnor = λab . Not (Xor a b) = BBB Not Xor = BBB C Xor
 */
inline const auto Xnor = coherseBool ^= combinator::C ^= Xor;

/**
 * Nand = BBB C And
 * See Xor -> Xnor for the derivation
 */
inline const auto Nand = coherseBool ^= combinator::C ^= And;

/**
 * Nor = BBB C Or
 * See Xor -> Xnor for the derivation
 */
inline const auto Nor = coherseBool ^= combinator::C ^= Or;

inline bool toBool(const lambda &b) {
  if (!b.boolValue())
    throw std::logic_error("toBool: input is not a boolean marker");
  return coherseBool(b).boolValue().value();
}

inline auto fromBool(bool b) { return b ? True : False; }
} // namespace boolean

inline lambda::lambda(bool x) { *this = boolean::fromBool(x); }