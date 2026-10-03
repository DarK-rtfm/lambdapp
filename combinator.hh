#pragma once

#include "lambda.hh"
#include "lambda_macros.hpp"
// inline application — left associative
inline lambda operator*(const lambda &f, const lambda &x) { return f(x); }

namespace combinator {
/**
 * I = λx.x
 */
inline const lambda I = λ(x, x);

/**
 * K = λab.a
 */
inline const lambda K = λ(a, b, a);

/**
 * KI = K I = λab.b
 */
inline const lambda KI = K * I;

/**
 * C = λfab.f b a
 */
inline const lambda C = λ(f, a, b, f * b * a);

/**
 * B = λfgx.f (g x)
 */
inline const lambda B = λ(f, g, x, f *(g *x));

/**
 * BBB = B B B
 */
inline const lambda BBB = B * B * B;

/**
 * V = λabf.f a b
 */
inline const lambda V = λ(a, b, f, f * a * b);

/**
 * S = λuvw.u w (v w)
 */
inline const lambda S = λ(u, v, w, u * w * (v * w));

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

// inline composition (B) — lower precedence than application
inline lambda operator|=(const lambda &f, const lambda &g) {
  return combinator::B * f * g;
}

// inline BBB
inline lambda operator^=(const lambda &f, const lambda &g) {
  return combinator::BBB * f * g;
}

// inline virio / pair / list constructor (V) — right associative
inline const auto operator%=(lambda hd, lambda tl) {
  return lambda::list(combinator::V * hd * tl);
}