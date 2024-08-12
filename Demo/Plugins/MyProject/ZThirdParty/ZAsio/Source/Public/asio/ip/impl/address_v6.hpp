//
// ip/impl/address_v6.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_IP_IMPL_ADDRESS_V6_HPP
#define ASIO_IP_IMPL_ADDRESS_V6_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if !defined(ASIO_NO_IOSTREAM)

#include "asio/detail/throw_error.hpp"

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace ip {

#if !defined(ASIO_NO_DEPRECATED)

inline address_v6 address_v6::from_string(const char* str)
{
  return zasio::ip::make_address_v6(str);
}

inline address_v6 address_v6::from_string(
    const char* str, zasio::error_code& ec)
{
  return zasio::ip::make_address_v6(str, ec);
}

inline address_v6 address_v6::from_string(const std::string& str)
{
  return zasio::ip::make_address_v6(str);
}

inline address_v6 address_v6::from_string(
    const std::string& str, zasio::error_code& ec)
{
  return zasio::ip::make_address_v6(str, ec);
}

#endif // !defined(ASIO_NO_DEPRECATED)

template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& os, const address_v6& addr)
{
  return os << addr.to_string().c_str();
}

} // namespace ip
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // !defined(ASIO_NO_IOSTREAM)

#endif // ASIO_IP_IMPL_ADDRESS_V6_HPP
