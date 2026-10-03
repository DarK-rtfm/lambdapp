#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

class lambda {
  struct state {
    std::function<lambda(lambda)> f;
    std::function<lambda()> thunk;
    std::shared_ptr<lambda> value;
    std::optional<int> int_value;
    std::optional<bool> bool_value;
    bool isList = false;
    bool isPair = false;

    state(std::function<lambda(lambda)> f, std::optional<int> int_value,
          std::optional<bool> bool_value, bool isList, bool isPair)
        : f(std::move(f)), int_value(int_value), bool_value(bool_value),
          isList(isList), isPair(isPair) {}

    explicit state(std::function<lambda()> thunk) : thunk(std::move(thunk)) {}
  };

  std::shared_ptr<state> state_;

  lambda(std::function<lambda(lambda)> f, std::optional<int> int_value,
         std::optional<bool> bool_value, bool isList, bool isPair)
      : state_(std::make_shared<state>(std::move(f), int_value, bool_value,
                                       isList, isPair)) {}

  explicit lambda(std::function<lambda()> thunk)
      : state_(std::make_shared<state>(std::move(thunk))) {}

  const lambda &resolve() const {
    if (state_->thunk && !state_->value) {
      state_->value = std::make_shared<lambda>(state_->thunk());
      state_->thunk = {};
    }
    return state_->value ? state_->value->resolve() : *this;
  }

  lambda apply(lambda x) const {
    const lambda &resolved = resolve();
    if (!resolved.state_->f) {
      throw std::logic_error("attempted to apply a non-function lambda");
    }
    return resolved.state_->f(std::move(x));
  }

public:
  lambda(const lambda &) = default;
  lambda(lambda &&) = default;
  lambda &operator=(const lambda &) = default;
  lambda &operator=(lambda &&) = default;

  template <class F,
            std::enable_if_t<!std::is_same_v<std::decay_t<F>, lambda> &&
                                 std::is_invocable_r_v<lambda, F &, lambda>,
                             int> = 0>
  lambda(F &&f)
      : state_(std::make_shared<state>(
            std::function<lambda(lambda)>(std::forward<F>(f)), std::nullopt,
            std::nullopt, false, false)) {}

  static lambda integer(int value, lambda f) {
    return lambda{f, value, std::nullopt, false, false};
  }

  static lambda boolean(bool value, lambda f) {
    return lambda{f, std::nullopt, value, false, false};
  }
  static lambda list(lambda f) {
    return lambda{f, std::nullopt, std::nullopt, true, false};
  }
  static lambda pair(lambda f) {
    return lambda{f, std::nullopt, std::nullopt, false, true};
  }

  const std::optional<int> &intValue() const {
    return resolve().state_->int_value;
  }

  const std::optional<bool> &boolValue() const {
    return resolve().state_->bool_value;
  }
  const bool isList() const { return resolve().state_->isList; }
  const bool isPair() const { return resolve().state_->isPair; }

  lambda operator()(lambda x) const {
    const lambda function = *this;
    return lambda{[function, x = std::move(x)]() mutable {
      return function.apply(std::move(x));
    }};
  }

  lambda(int x);
  lambda(bool x);
  std::string print_list(const lambda &list) const;
  std::string print_pair(const lambda &pair) const;
};

inline std::string toString(const lambda &l) {
  if (l.intValue()) {
    return std::to_string(l.intValue().value());
  } else if (l.boolValue()) {
    return l.boolValue().value() ? "True" : "False";
  } else if (l.isPair()) {
    return l.print_pair(l);
  } else if (l.isList()) {
    return l.print_list(l);
  } else {
    return "Unknown";
  }
}

inline std::ostream &operator<<(std::ostream &os, const lambda &l) {
  os << toString(l);
  return os;
}