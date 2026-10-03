#pragma once

#include "lambda.hh"
namespace combinator {
/**
 * I = λx.x
 */
inline const lambda I{[](lambda x) { return x; }};

/**
 * K = λab.a
 */
inline const lambda K{
    [](lambda a) { return lambda{[a](lambda) { return a; }}; }};

/**
 * KI = K I = λab.b
 */
inline const lambda KI{
    [](lambda) { return lambda{[](lambda b) { return b; }}; }};

/**
 * C = λfab.f b a
 */
inline const lambda C{[](lambda f) {
  return lambda{
      [f](lambda a) { return lambda{[f, a](lambda b) { return f(b)(a); }}; }};
}};

/**
 * B = λfgx.f (g x)
 */
inline const lambda B{[](lambda f) {
  return lambda{
      [f](lambda g) { return lambda{[f, g](lambda a) { return f(g(a)); }}; }};
}};

/**
 * BBB = B B B
 */
inline const lambda BBB = B(B)(B);

/**
 * V = λabf.f a b
 */
inline const lambda V{[](lambda a) {
  return lambda{
      [a](lambda b) { return lambda{[a, b](lambda f) { return f(a)(b); }}; }};
}};

/**
 * S = λuvw.u w (v w)
 */
inline const lambda S{[](lambda u) {
  return lambda{[u](lambda v) {
    return lambda{[u, v](lambda w) { return u(w)(v(w)); }};
  }};
}};

/**
 * Th = λaf.f a
 */
inline const lambda Th{
    [](lambda a) { return lambda{[a](lambda f) { return f(a); }}; }};

/**
 * U = λfghn = f (g n) (h n)
 * !! Non-standard, a kind of diagonal combinator, basically LiftA2 in haskell.
 */
inline const lambda U{[](lambda f) {
  return lambda{[f](lambda g) {
    return lambda{[f, g](lambda h) {
      return lambda{[f, g, h](lambda n) { return f(g(n))(h(n)); }};
    }};
  }};
}};

/**
 * Y = λf.(λx.f (x x)) (λx.f (x x))
 */
inline const lambda Y{[](lambda f) {
  return lambda{[f](lambda x) { return f(x(x)); }}(
      lambda{[f](lambda x) { return f(x(x)); }});
}};

} // namespace combinator

// inline application — left associative
inline lambda operator*(const lambda &f, const lambda &x) { return f(x); }

// inline composition (B) — lower precedence than application
inline lambda operator|=(const lambda &f, const lambda &g) {
  return combinator::B(f)(g);
}

// inline BBB
inline lambda operator^=(const lambda &f, const lambda &g) {
  return combinator::BBB(f)(g);
}

// inline virio / pair / list constructor (V) — right associative
inline const auto operator%=(lambda hd, lambda tl) {
  return lambda::list(combinator::V(hd)(tl));
}