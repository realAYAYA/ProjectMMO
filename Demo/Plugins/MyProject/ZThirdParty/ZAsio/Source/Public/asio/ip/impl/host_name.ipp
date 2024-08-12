//
// ip/impl/host_name.ipp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_IP_IMPL_HOST_NAME_IPP
#define ASIO_IP_IMPL_HOST_NAME_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/socket_ops.hpp"
#include "asio/detail/throw_error.hpp"
#include "asio/detail/winsock_init.hpp"
#include "asio/ip/host_name.hpp"

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace ip {

std::string host_name()
{
  char name[1024];
  zasio::error_code ec;
  if (zasio::detail::socket_ops::gethostname(name, sizeof(name), ec) != 0)
  {
    zasio::detail::throw_error(ec);
    return std::string();
  }
  return std::string(name);
}

std::string host_name(zasio::error_code& ec)
{
  char name[1024];
  if (zasio::detail::socket_ops::gethostname(name, sizeof(name), ec) != 0)
    return std::string();
  return std::string(name);
}

} // namespace ip
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_IP_IMPL_HOST_NAME_IPP
