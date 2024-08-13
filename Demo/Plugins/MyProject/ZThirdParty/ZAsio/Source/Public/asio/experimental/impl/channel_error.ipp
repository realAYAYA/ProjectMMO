//
// experimental/impl/channel_error.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXPERIMENTAL_IMPL_CHANNEL_ERROR_IPP
#define ASIO_EXPERIMENTAL_IMPL_CHANNEL_ERROR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/experimental/channel_error.hpp"

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace experimental {
namespace error {
namespace detail {

class channel_category : public zasio::error_category
{
public:
  const char* name() const ASIO_ERROR_CATEGORY_NOEXCEPT
  {
    return "asio.channel";
  }

  std::string message(int value) const
  {
    switch (value)
    {
    case channel_closed: return "Channel closed";
    case channel_cancelled: return "Channel cancelled";
    default: return "asio.channel error";
    }
  }
};

} // namespace detail

const zasio::error_category& get_channel_category()
{
  static detail::channel_category instance;
  return instance;
}

} // namespace error
} // namespace experimental
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXPERIMENTAL_IMPL_CHANNEL_ERROR_IPP