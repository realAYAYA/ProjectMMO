//
// ssl/detail/shutdown_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP
#define ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/ssl/detail/engine.hpp"

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace ssl {
namespace detail {

class shutdown_op
{
public:
  static ASIO_CONSTEXPR const char* tracking_name()
  {
    return "ssl::stream<>::async_shutdown";
  }

  engine::want operator()(engine& eng,
      zasio::error_code& ec,
      std::size_t& bytes_transferred) const
  {
    bytes_transferred = 0;
    return eng.shutdown(ec);
  }

  template <typename Handler>
  void call_handler(Handler& handler,
      const zasio::error_code& ec,
      const std::size_t&) const
  {
    if (ec == zasio::error::eof)
    {
      // The engine only generates an eof when the shutdown notification has
      // been received from the peer. This indicates that the shutdown has
      // completed successfully, and thus need not be passed on to the handler.
      ASIO_MOVE_OR_LVALUE(Handler)(handler)(zasio::error_code());
    }
    else
    {
      ASIO_MOVE_OR_LVALUE(Handler)(handler)(ec);
    }
  }
};

} // namespace detail
} // namespace ssl
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_SSL_DETAIL_SHUTDOWN_OP_HPP
