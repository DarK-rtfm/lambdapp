// Taken from
// https://github.com/itsthatMatthew/funcpp/blob/main/lambda_macros.hpp
// Modified such that the outer lambda does not capture.

/// Lambda calculus(-ish) macro definitions to achieve a notation as close to
/// the mathematical language as possible. This was my first shot, trying to
/// mimic right-associativity with variadic macro arguments (up to 8 of them),
/// where the last one resolves to the lambda's body, with the first n becoming
/// the parameters.
///
/// A domain specific language, you say? LCDSL? FCPPDSL? I don't even know them!

#pragma once // For good measure

// A lambda is just a closure with a specific parameter returning the expression
#define LAMBDA(x, E) [=](auto x) -> decltype(auto) { return E; }
#define LAMBDA_x(x, E) [](auto x) -> decltype(auto) { return E; }
// ^-- The evil capture-default by value is rather necessary
// for the right-associativity to function correctly (among
// other things)

// Lambda parameter list "overloads" for specific parameter counts
#define LAMBDA_IMPL_2(x, E) LAMBDA(x, E)
#define LAMBDA_IMPL_3(x, ...) LAMBDA(x, LAMBDA_IMPL_2(__VA_ARGS__))
#define LAMBDA_IMPL_4(x, ...) LAMBDA(x, LAMBDA_IMPL_3(__VA_ARGS__))
#define LAMBDA_IMPL_5(x, ...) LAMBDA(x, LAMBDA_IMPL_4(__VA_ARGS__))
#define LAMBDA_IMPL_6(x, ...) LAMBDA(x, LAMBDA_IMPL_5(__VA_ARGS__))
#define LAMBDA_IMPL_7(x, ...) LAMBDA(x, LAMBDA_IMPL_6(__VA_ARGS__))

#define LAMBDA_IMPL_x_2(x, E) LAMBDA_x(x, E)
#define LAMBDA_IMPL_x_3(x, ...) LAMBDA_x(x, LAMBDA_IMPL_2(__VA_ARGS__))
#define LAMBDA_IMPL_x_4(x, ...) LAMBDA_x(x, LAMBDA_IMPL_3(__VA_ARGS__))
#define LAMBDA_IMPL_x_5(x, ...) LAMBDA_x(x, LAMBDA_IMPL_4(__VA_ARGS__))
#define LAMBDA_IMPL_x_6(x, ...) LAMBDA_x(x, LAMBDA_IMPL_5(__VA_ARGS__))
#define LAMBDA_IMPL_x_7(x, ...) LAMBDA_x(x, LAMBDA_IMPL_6(__VA_ARGS__))
#define LAMBDA_IMPL_x_8(x, ...) LAMBDA_x(x, LAMBDA_IMPL_7(__VA_ARGS__))

// Bog-standard argument counter to resolve up to 8 ones
#define LAMBDA_ARGS_COUNTER(...)                                               \
  LAMBDA_ARGS_SEQUENCE(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define LAMBDA_ARGS_SEQUENCE(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

// Lambda dispatch with macro concatenation based on the number of parameters
#define LAMBDA_DISPATCH_x(N, ...) LAMBDA_DISPATCH_IMPL_x(N, __VA_ARGS__)
#define LAMBDA_DISPATCH_IMPL_x(N, ...) LAMBDA_IMPL_x_##N(__VA_ARGS__)
#define LAMBDA_DISPATCH(N, ...) LAMBDA_DISPATCH_IMPL(N, __VA_ARGS__)
#define LAMBDA_DISPATCH_IMPL(N, ...) LAMBDA_IMPL_##N(__VA_ARGS__)

// v-- Evil and intimidating greek letter
#define λ(...) LAMBDA_DISPATCH_x(LAMBDA_ARGS_COUNTER(__VA_ARGS__), __VA_ARGS__)
#define λλ(...) LAMBDA_DISPATCH(LAMBDA_ARGS_COUNTER(__VA_ARGS__), __VA_ARGS__)