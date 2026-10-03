#pragma once

#include "lambda.hh"
#include "lambda_macros.hpp"
// inline application — left associative
inline lambda operator*(const lambda &f, const lambda &x) { return f(x); }
// inline application -- right associative
inline lambda operator*=(const lambda &f, const lambda &x) { return f(x); }

namespace combinator {
/**
 * S = λuvw.u w (v w)
 */
inline const lambda S = λ(u, v, w, u *w *= v * w);

/**
 * K = λab.a
 */
inline const lambda K = λ(a, b, a);

/**
 * I = λx.x
 *
 * S K K = λa. K a (K a) = λa. a = I
 */
inline const lambda I = S * K * K;

/**
 * B = λfgx.f (g x)
 *
 * S(KS)K = λa. K S a (K a) = λa. S (K a)
 * = λabc. S (k a) b c = λabc. (K a) c (b c) = λabc. a (b c) = B
 */
inline const lambda B = S * (K * S) * K;

/**
 * B2 = B B
 */
inline const lambda B2 = B * B;
/**
 * B3 = B B B
 */
inline const lambda B3 = B * B * B;
/**
 * KI = K I = λab.b
 */
inline const lambda KI = K * I;

/**
 * C = λfab.f b a
 */
inline const lambda C = λ(f, a, b, f * b * a);

/**
 * V = λabf.f a b
 */
inline const lambda V = λ(a, b, f, f * a * b);

/**
 * Th = λaf.f a
 */
inline const lambda Th = λ(a, f, f *a);

/**
 * U = λfghn = f (g n) (h n)
 * !! Non-standard, a kind of diagonal combinator, basically LiftA2 in haskell.
 */
inline const lambda U = λ(f, g, h, n, f *(g *n) * (h * n));

/**
 * Y = λf.(λx.f (x x)) (λx.f (x x))
 */
inline const lambda Y = λ(f, (λλ(x, f *(x *x))) * (λλ(x, f *(x *x))));

} // namespace combinator

// inline composition (B) -- right associative
inline lambda operator|=(const lambda &f, const lambda &g) {
  return combinator::B * f * g;
}

// inline B3
inline lambda operator^=(const lambda &f, const lambda &g) {
  return combinator::B3 * f * g;
}

// inline list constructor (V) — right associative
inline const auto operator%=(lambda hd, lambda tl) {
  return lambda::list(combinator::V * hd * tl);
}

// Pair constructor, kept distinct from the list constructor cuz marking, left
// associative.
inline const auto operator,(lambda first, lambda second) {
  return lambda::pair(combinator::V * first * second);
}