#pragma once
#include "boolean.hh"
#include "combinator.hh"
// Én church-encoding hívő vagyok

/**
 * N számra
 *   N f a = f (f (f ... (f a))) -- N-szer hívva f-et a-ra
 */
namespace number {
// force to number in case n doesnt originate from a lambda::integer
// instance
inline lambda coherseInt(const lambda &n) {

  const lambda increment{[](lambda value) {
    return lambda::integer(value.intValue().value() + 1,
                           combinator::S * combinator::B * value);
  }};

  return n(increment)(lambda::integer(0, combinator::KI));
}

// ---vvv--- abstractions implemented
/**
 * Zero = λfx.x = KI (= False)
 */
inline const auto Zero = lambda::integer(0, combinator::KI);

/**
 * Succ = λnfx.f (n f x) = λnf.B f (nf) = λn. S B n = S B
 */
inline const auto Succ = coherseInt |= combinator::S * combinator::B;

/**
 * Phi = λp. Pair (Snd p) (Succ (Snd p)) = λp. S Pair Succ (Snd p) = λp. B (S
 * Pair Succ) Snd p = B (S Pair Succ) Snd = B (S V Succ) (Th KI)
 *
 * Pred = λn. Fst (n Phi (Pair 0 0)) = λn . Fst (V Phi (Pair 0 0) n) = λn. B Fst
 * (V Phi (Pair 0 0)) n = B Fst (V Phi (Pair 0 0)) = B (Th K) (V Phi (V (KI)
 * (KI))) = ... phi behelyettesitve inline
 */
inline const auto Pred = combinator::B(combinator::Th(combinator::K))(
    combinator::V(combinator::B(combinator::S(combinator::V)(Succ))(
        combinator::Th(combinator::KI)))(combinator::V(Zero)(Zero)));
// inline const auto Pred = combinator::Th * combinator::K |=
//     (combinator::S *combinator::V *Succ |= combinator::Th * combinator::KI)
//     %= Zero %= Zero;

/**
 * Add = λmnf.(n Succ m) f = λmn. n Succ m = C (λnm. n Succ m) = C (λn. n Succ)
 * = C (Th Succ)
 */
inline const auto Add = coherseInt ^= combinator::C * (combinator::Th * Succ);

/**
 * Sub = λmn. m Pred n = λmn. n Pred m = C (λnm. n Pred m) = C (λn. n Pred)
 *   = C (Th Pred)
 */
inline const lambda Sub = coherseInt ^= combinator::C * (combinator::Th * Pred);

/**
 * Mult = λnkf.n (k f) = B
 */
inline const auto Mul = coherseInt ^= combinator::B;

/**
 * Pow = λmn.n m = Th
 */
inline const auto Pow = coherseInt ^= combinator::Th;

/**
 * is0 = λn.n K(False) True = V (K(False)) True
 */
inline const lambda Is0 =
    combinator::V * (combinator::K * boolean::False) * boolean::True;

/**
 * leq = λmn.Is0 (Sub m n) = BBB Is0 Sub
 */
inline const lambda Leq = Is0 ^= Sub;

/**
 * geq = C leq
 */
inline const lambda Geq = combinator::C * Leq;

/**
 * eq = λmn.And (geq m n) (leq m n)
 * = λmn. And (Th geq (V m n)) (Th leq (V m n))
 * = λmn. U And (Th geq) (Th leq) (V m n)
 * = λmn. BBB (U And (Th geq) (Th leq)) V
 */
inline const lambda Eq = combinator::U * boolean::And * combinator::Th(Geq) *
                         combinator::Th(Leq) ^= combinator::V;

/**
 * neq = λmn.Not (eq m n) = BBB Not geq leq
 *

 */
inline const lambda Neq = boolean::Not ^= Eq;

/**
 * gt = λmn.Not (leq m n) = BBB Not leq
 */
inline const lambda Gt = boolean::Not ^= Leq;

/**
 * lt = C gt
 */
inline const lambda Lt = combinator::C * Gt;

/**
 * fac_h = λself. λn. Is0 n 1 (Mul n (self (Pred n)))
 * fac =Y (λsn. Is0 n 1 (Mul n (s (Pred n))))
 */
inline const lambda fac =
    combinator::Y * λ(s, n, Is0 * n * Succ(Zero) * (Mul * n * s(Pred * n)));

/**
 fib_h = λs. λn. Is0 (Pred n) 1 (Add (s (Pred n)) (s (Pred (Pred n))))

 fib = Y fib_h
 */
inline const lambda fib =
    combinator::Y *
    λ(s, n,
      Is0 *Pred(n) * Succ(Zero) * (Add * s(Pred * n) * s(Pred * (Pred * n))));
// ---^^^--- abstractions implementated.

// ---vvv--- C++ <-> Church conversions
inline int toInt(const lambda &n) { return coherseInt(n).intValue().value(); }

inline lambda fromInt(int value) {
  if (value < 0) {
    throw std::invalid_argument("Church numerals cannot be negative");
  }

  auto result = number::Zero;
  for (int index = 0; index < value; ++index) {
    result = number::Succ(result);
  }
  return result;
}
// ---^^^--- C++ <-> Church conversions
} // namespace number

// ---vvv--- implicit conversion
inline lambda::lambda(int x) { *this = number::fromInt(x); }
// ---^^^--- implicit conversion
