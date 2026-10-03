#pragma once
#include "boolean.hh"
#include "combinator.hh"
#include "number.hh"
#include "pair.hh"

namespace list {
inline const auto Nil = lambda::list(combinator::K * boolean::True);

inline const auto Null =
    combinator::Th(combinator::K(combinator::K(boolean::False)));

inline const auto Cons = lambda::list |= pair::Pair;

inline const auto Hd = pair::Fst;

inline const auto Tl = pair::Snd;

/**
 * fold fun acc list = Null list acc (fold fun (fun acc (hd list)) (tl list))
 * fold = Y (λfold fun acc list. Null list acc (fold fun (fun acc (hd list))
 */
inline const auto Fold = combinator::Y([](lambda fold) {
  return [fold](lambda fun) {
    return [fold, fun](lambda acc) {
      return [fold, fun, acc](lambda list) {
        return Null * list * acc *
               (fold * fun * (fun(acc)(Hd(list))) * Tl(list));
      };
    };
  };
});

/**
 * Sum = Fold Add 0
 */
inline const auto Sum = Fold * number::Add * number::Zero;

/**
 * Len = Fold (B K succ) 0
 */
inline const auto Len = Fold * (combinator::K |= number::Succ) * number::Zero;

/**
 * Reverse = Fold (C V) Nil
 */
inline const auto Reverse = Fold * (combinator::C * combinator::V) * Nil;

/**
 * Take n= Fold (λacc l. Or (Eq n (Len acc)) (Null l) acc (Hd l %= acc)) Nil
 * ^ Not lazy :(
 * v Lazy version, unrelated:
 * Take = Y$ (λself. λn. λlist. Or (Is0 n) (Null list) (Nil) (Hd list %= self
 * (Pred n) (Tl list)))
 */
inline const auto Take =
    combinator::Y * λ(self, n, list,
                      boolean::Or *number::Is0(n) * Null(list) * Nil *
                          (Hd(list) %= self(number::Pred * n)(Tl(list))));

/**
 * Drop = Y$ (λself. λn. λlist. Or (Is0 n) (Null list) (list) (self (Pred n) (Tl
 * list)))
 */
inline const auto Drop =
    combinator::Y * λ(self, n, list,
                      boolean::Or *number::Is0(n) * Null(list) * list *
                          self(number::Pred * n)(Tl(list)));

/**
 * Inflist = (λsn. n %= (s (Succ n))) 0
 */
inline const auto Inflist =
    combinator::Y * λ(s, n, n %= (s(number::Succ * n))) * number::Zero;

/**
 * Range m n= (Drop m |= Take n ) Inflist
 */
inline const auto Range = λ(m, n, (Drop(m) |= Take(n)) * Inflist);

/*
Iota = λn. Take n Inflist = λn. C Take Inflist n = C Take Inflist
*/
inline const auto Iota = combinator::C * list::Take * list::Inflist;

/**
 * Zipwith = \self f l1 l2 = Or (Null l1) (Null l2) Nil (f (Hd l1) (Hd l2) %=
 * self f (Tl l1) (Tl l2))
 */
inline const auto Zipwith =
    combinator::Y * λ(self, f, l1, l2,
                      boolean::Or *Null(l1) * Null(l2) * Nil *
                          (f * Hd(l1) * Hd(l2) %= self(f)(Tl(l1))(Tl(l2))));

/**
 * Zip = Zipwith V
 * (implemented as Zipwith (λab. a ^ b)) so it gets marked as a pair.
 */
inline const auto Zip = Zipwith * (λ(a, b, (a, b)));

/**
 * Repeat = Y$ λsx. x %= (s x)
 */
inline const auto Repeat = combinator::Y * λ(s, x, x %= (s * x));

/**
 * Withindex = C Zip InfList
 */
inline const auto Withindex = λ(l, Zip(l)(Inflist));

/**
 * Map = ZipWith I |= Repeat
 */
inline const auto Map = Zipwith *combinator::I |= Repeat;
} // namespace list

inline std::string print_list(const lambda &lst) {
  std::string result;
  const auto is_nil = boolean::toBool(list::Null(lst));
  if (is_nil) {
    result = "Nil";
  } else {
    const auto head = list::Hd(lst);
    const auto tail = list::Tl(lst);
    result = toString(head);
    result = "(" + result + ") %= " + print_list(tail);
  }
  return result;
}

inline std::string lambda::print_list(const lambda &list) const {
  return ::print_list(list);
}