// MPark.Patterns
//
// Copyright Michael Park, 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef MPARK_PATTERNS_BIND_HPP
#define MPARK_PATTERNS_BIND_HPP

#include <utility>

#include "wildcard.hpp"

namespace mpark::patterns {

  template <typename Pattern>
  struct Arg { const Pattern &pattern; };

  template <>
  struct Arg<Wildcard> {
    template <typename Pattern_>
    auto operator()(const Pattern_ &pattern_) const noexcept {
      return Arg<Pattern_>{pattern_};
    }

    const Wildcard &pattern = _;
  };

  using placeholder = Arg<Wildcard>;

  inline constexpr Arg<Wildcard> arg{};

  template <typename Pattern, typename Value, typename F>
  auto matches(const Arg<Pattern> &arg, Value &&value, F &&f) {
    return matches(
        arg.pattern, std::forward<Value>(value), [&](auto &&... args) {
          return match_invoke(std::forward<F>(f),
                              std::forward<Value>(value),
                              std::forward<decltype(args)>(args)...);
        });
  }

}  // namespace mpark::patterns

#endif  // MPARK_PATTERNS_BIND_HPP
