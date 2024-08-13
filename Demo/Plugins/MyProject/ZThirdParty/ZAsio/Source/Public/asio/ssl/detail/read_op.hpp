//
// ssl/detail/read_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_SSL_DETAIL_READ_OP_HPP
#define ASIO_SSL_DETAIL_READ_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#include "asio/detail/buffer_sequence_adapter.hpp"
#include "asio/ssl/detail/engine.hpp"

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace ssl {
namespace detail {

template <typename MutableBufferSequence>
class read_op
{
public:
  static ASIO_CONSTEXPR const char* tracking_name()
  {
    return "ssl::stream<>::async_read_some";
  }

  read_op(const MutableBufferSequence& buffers)
    : buffers_(buffers)
  {
  }

  engine::want operator()(engine& eng,
      zasio::error_code& ec,
      std::size_t& bytes_transferred) const
  {
    zasio::mutable_buffer buffer =
      zasio::detail::buffer_sequence_adapter<zasio::mutable_buffer,
        MutableBufferSequence>::first(buffers_);

    return eng.read(buffer, ec, bytes_transferred);
  }

  template <typename Handler>
  void call_handler(Handler& handler,
      const zasio::error_code& ec,
      const std::size_t& bytes_transferred) const
  {
    ASIO_MOVE_OR_LVALUE(Handler)(handler)(ec, bytes_transferred);
  }

private:
  MutableBufferSequence buffers_;
};

} // namespace detail
} // namespace ssl
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_SSL_DETAIL_READ_OP_HPP