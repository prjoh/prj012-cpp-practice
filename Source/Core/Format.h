/*
 * Format.h - fmt wrappers
 * Copyright (c) 2024 Johannes Przybilla. All Rights Reserved.
 */

#pragma once

#include <string_view>

#include <ThirdParty/fmt/include/fmt/color.h>
#include <ThirdParty/fmt/include/fmt/core.h>

namespace zv
{
  using FormatContext = fmt::format_context;
  using FormatArgs = fmt::format_args;
  using FormatColor = fmt::color;

  template<typename Context, typename ...Args>
  using FormatArgStorage = fmt::format_arg_store<Context, fmt::remove_cvref_t<Args>...>;

  template<typename ...Args>
  FormatArgStorage<FormatContext, fmt::remove_cvref_t<Args>...> make_format_args(Args&&... args) {
    return fmt::make_format_args(args...);
  }

  inline std::string vformat(std::string_view fmt, FormatArgs args) {
    return fmt::vformat(fmt, args);
  }
}