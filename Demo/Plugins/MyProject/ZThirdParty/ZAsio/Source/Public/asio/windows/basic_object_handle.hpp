//
// windows/basic_object_handle.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2011 Boris Schaeling (boris@highscore.de)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_WINDOWS_BASIC_OBJECT_HANDLE_HPP
#define ASIO_WINDOWS_BASIC_OBJECT_HANDLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"

#if defined(ASIO_HAS_WINDOWS_OBJECT_HANDLE) \
  || defined(GENERATING_DOCUMENTATION)

#include "asio/any_io_executor.hpp"
#include "asio/async_result.hpp"
#include "asio/detail/io_object_impl.hpp"
#include "asio/detail/throw_error.hpp"
#include "asio/detail/win_object_handle_service.hpp"
#include "asio/error.hpp"
#include "asio/execution_context.hpp"

#if defined(ASIO_HAS_MOVE)
# include <utility>
#endif // defined(ASIO_HAS_MOVE)

#include "asio/detail/push_options.hpp"

namespace zasio {
namespace windows {

/// Provides object-oriented handle functionality.
/**
 * The windows::basic_object_handle class provides asynchronous and blocking
 * object-oriented handle functionality.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
template <typename Executor = any_io_executor>
class basic_object_handle
{
private:
  class initiate_async_wait;

public:
  /// The type of the executor associated with the object.
  typedef Executor executor_type;

  /// Rebinds the handle type to another executor.
  template <typename Executor1>
  struct rebind_executor
  {
    /// The handle type when rebound to the specified executor.
    typedef basic_object_handle<Executor1> other;
  };

  /// The native representation of a handle.
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined native_handle_type;
#else
  typedef zasio::detail::win_object_handle_service::native_handle_type
    native_handle_type;
#endif

  /// An object handle is always the lowest layer.
  typedef basic_object_handle lowest_layer_type;

  /// Construct an object handle without opening it.
  /**
   * This constructor creates an object handle without opening it.
   *
   * @param ex The I/O executor that the object handle will use, by default, to
   * dispatch handlers for any asynchronous operations performed on the
   * object handle.
   */
  explicit basic_object_handle(const executor_type& ex)
    : impl_(0, ex)
  {
  }

  /// Construct an object handle without opening it.
  /**
   * This constructor creates an object handle without opening it.
   *
   * @param context An execution context which provides the I/O executor that
   * the object handle will use, by default, to dispatch handlers for any
   * asynchronous operations performed on the object handle.
   */
  template <typename ExecutionContext>
  explicit basic_object_handle(ExecutionContext& context,
      typename constraint<
        is_convertible<ExecutionContext&, execution_context&>::value,
        defaulted_constraint
      >::type = defaulted_constraint())
    : impl_(0, 0, context)
  {
  }

  /// Construct an object handle on an existing native handle.
  /**
   * This constructor creates an object handle object to hold an existing native
   * handle.
   *
   * @param ex The I/O executor that the object handle will use, by default, to
   * dispatch handlers for any asynchronous operations performed on the
   * object handle.
   *
   * @param native_handle The new underlying handle implementation.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  basic_object_handle(const executor_type& ex,
      const native_handle_type& native_handle)
    : impl_(0, ex)
  {
    zasio::error_code ec;
    impl_.get_service().assign(impl_.get_implementation(), native_handle, ec);
    zasio::detail::throw_error(ec, "assign");
  }

  /// Construct an object handle on an existing native handle.
  /**
   * This constructor creates an object handle object to hold an existing native
   * handle.
   *
   * @param context An execution context which provides the I/O executor that
   * the object handle will use, by default, to dispatch handlers for any
   * asynchronous operations performed on the object handle.
   *
   * @param native_handle The new underlying handle implementation.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  template <typename ExecutionContext>
  basic_object_handle(ExecutionContext& context,
      const native_handle_type& native_handle,
      typename constraint<
        is_convertible<ExecutionContext&, execution_context&>::value
      >::type = 0)
    : impl_(0, 0, context)
  {
    zasio::error_code ec;
    impl_.get_service().assign(impl_.get_implementation(), native_handle, ec);
    zasio::detail::throw_error(ec, "assign");
  }

#if defined(ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)
  /// Move-construct an object handle from another.
  /**
   * This constructor moves an object handle from one object to another.
   *
   * @param other The other object handle object from which the move will
   * occur.
   *
   * @note Following the move, the moved-from object is in the same state as if
   * constructed using the @c basic_object_handle(const executor_type&)
   * constructor.
   */
  basic_object_handle(basic_object_handle&& other)
    : impl_(std::move(other.impl_))
  {
  }

  /// Move-assign an object handle from another.
  /**
   * This assignment operator moves an object handle from one object to another.
   *
   * @param other The other object handle object from which the move will
   * occur.
   *
   * @note Following the move, the moved-from object is in the same state as if
   * constructed using the @c basic_object_handle(const executor_type&)
   * constructor.
   */
  basic_object_handle& operator=(basic_object_handle&& other)
  {
    impl_ = std::move(other.impl_);
    return *this;
  }

  // All handles have access to each other's implementations.
  template <typename Executor1>
  friend class basic_object_handle;

  /// Move-construct an object handle from a handle of another executor type.
  /**
   * This constructor moves an object handle from one object to another.
   *
   * @param other The other object handle object from which the move will
   * occur.
   *
   * @note Following the move, the moved-from object is in the same state as if
   * constructed using the @c basic_object_handle(const executor_type&)
   * constructor.
   */
  template<typename Executor1>
  basic_object_handle(basic_object_handle<Executor1>&& other,
      typename constraint<
        is_convertible<Executor1, Executor>::value,
        defaulted_constraint
      >::type = defaulted_constraint())
    : impl_(std::move(other.impl_))
  {
  }

  /// Move-assign an object handle from a handle of another executor type.
  /**
   * This assignment operator moves an object handle from one object to another.
   *
   * @param other The other object handle object from which the move will
   * occur.
   *
   * @note Following the move, the moved-from object is in the same state as if
   * constructed using the @c basic_object_handle(const executor_type&)
   * constructor.
   */
  template<typename Executor1>
  typename constraint<
    is_convertible<Executor1, Executor>::value,
    basic_object_handle&
  >::type operator=(basic_object_handle<Executor1>&& other)
  {
    impl_ = std::move(other.impl_);
    return *this;
  }
#endif // defined(ASIO_HAS_MOVE) || defined(GENERATING_DOCUMENTATION)

  /// Get the executor associated with the object.
  const executor_type& get_executor() ASIO_NOEXCEPT
  {
    return impl_.get_executor();
  }

  /// Get a reference to the lowest layer.
  /**
   * This function returns a reference to the lowest layer in a stack of
   * layers. Since an object handle cannot contain any further layers, it simply
   * returns a reference to itself.
   *
   * @return A reference to the lowest layer in the stack of layers. Ownership
   * is not transferred to the caller.
   */
  lowest_layer_type& lowest_layer()
  {
    return *this;
  }

  /// Get a const reference to the lowest layer.
  /**
   * This function returns a const reference to the lowest layer in a stack of
   * layers. Since an object handle cannot contain any further layers, it simply
   * returns a reference to itself.
   *
   * @return A const reference to the lowest layer in the stack of layers.
   * Ownership is not transferred to the caller.
   */
  const lowest_layer_type& lowest_layer() const
  {
    return *this;
  }

  /// Assign an existing native handle to the handle.
  /*
   * This function opens the handle to hold an existing native handle.
   *
   * @param handle A native handle.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  void assign(const native_handle_type& handle)
  {
    zasio::error_code ec;
    impl_.get_service().assign(impl_.get_implementation(), handle, ec);
    zasio::detail::throw_error(ec, "assign");
  }

  /// Assign an existing native handle to the handle.
  /*
   * This function opens the handle to hold an existing native handle.
   *
   * @param handle A native handle.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  ASIO_SYNC_OP_VOID assign(const native_handle_type& handle,
      zasio::error_code& ec)
  {
    impl_.get_service().assign(impl_.get_implementation(), handle, ec);
    ASIO_SYNC_OP_VOID_RETURN(ec);
  }

  /// Determine whether the handle is open.
  bool is_open() const
  {
    return impl_.get_service().is_open(impl_.get_implementation());
  }

  /// Close the handle.
  /**
   * This function is used to close the handle. Any asynchronous read or write
   * operations will be cancelled immediately, and will complete with the
   * zasio::error::operation_aborted error.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  void close()
  {
    zasio::error_code ec;
    impl_.get_service().close(impl_.get_implementation(), ec);
    zasio::detail::throw_error(ec, "close");
  }

  /// Close the handle.
  /**
   * This function is used to close the handle. Any asynchronous read or write
   * operations will be cancelled immediately, and will complete with the
   * zasio::error::operation_aborted error.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  ASIO_SYNC_OP_VOID close(zasio::error_code& ec)
  {
    impl_.get_service().close(impl_.get_implementation(), ec);
    ASIO_SYNC_OP_VOID_RETURN(ec);
  }

  /// Get the native handle representation.
  /**
   * This function may be used to obtain the underlying representation of the
   * handle. This is intended to allow access to native handle functionality
   * that is not otherwise provided.
   */
  native_handle_type native_handle()
  {
    return impl_.get_service().native_handle(impl_.get_implementation());
  }

  /// Cancel all asynchronous operations associated with the handle.
  /**
   * This function causes all outstanding asynchronous read or write operations
   * to finish immediately, and the handlers for cancelled operations will be
   * passed the zasio::error::operation_aborted error.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  void cancel()
  {
    zasio::error_code ec;
    impl_.get_service().cancel(impl_.get_implementation(), ec);
    zasio::detail::throw_error(ec, "cancel");
  }

  /// Cancel all asynchronous operations associated with the handle.
  /**
   * This function causes all outstanding asynchronous read or write operations
   * to finish immediately, and the handlers for cancelled operations will be
   * passed the zasio::error::operation_aborted error.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  ASIO_SYNC_OP_VOID cancel(zasio::error_code& ec)
  {
    impl_.get_service().cancel(impl_.get_implementation(), ec);
    ASIO_SYNC_OP_VOID_RETURN(ec);
  }

  /// Perform a blocking wait on the object handle.
  /**
   * This function is used to wait for the object handle to be set to the
   * signalled state. This function blocks and does not return until the object
   * handle has been set to the signalled state.
   *
   * @throws zasio::system_error Thrown on failure.
   */
  void wait()
  {
    zasio::error_code ec;
    impl_.get_service().wait(impl_.get_implementation(), ec);
    zasio::detail::throw_error(ec, "wait");
  }

  /// Perform a blocking wait on the object handle.
  /**
   * This function is used to wait for the object handle to be set to the
   * signalled state. This function blocks and does not return until the object
   * handle has been set to the signalled state.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  void wait(zasio::error_code& ec)
  {
    impl_.get_service().wait(impl_.get_implementation(), ec);
  }

  /// Start an asynchronous wait on the object handle.
  /**
   * This function is be used to initiate an asynchronous wait against the
   * object handle. It is an initiating function for an @ref
   * asynchronous_operation, and always returns immediately.
   *
   * @param token The @ref completion_token that will be used to produce a
   * completion handler, which will be called when the wait completes.
   * Potential completion tokens include @ref use_future, @ref use_awaitable,
   * @ref yield_context, or a function object with the correct completion
   * signature. The function signature of the completion handler must be:
   * @code void handler(
   *   const zasio::error_code& error // Result of operation.
   * ); @endcode
   * Regardless of whether the asynchronous operation completes immediately or
   * not, the completion handler will not be invoked from within this function.
   * On immediate completion, invocation of the handler will be performed in a
   * manner equivalent to using zasio::post().
   *
   * @par Completion Signature
   * @code void(zasio::error_code) @endcode
   */
  template <
      ASIO_COMPLETION_TOKEN_FOR(void (zasio::error_code))
        WaitToken ASIO_DEFAULT_COMPLETION_TOKEN_TYPE(executor_type)>
  ASIO_INITFN_AUTO_RESULT_TYPE_PREFIX(WaitToken,
      void (zasio::error_code))
  async_wait(
      ASIO_MOVE_ARG(WaitToken) token
        ASIO_DEFAULT_COMPLETION_TOKEN(executor_type))
    ASIO_INITFN_AUTO_RESULT_TYPE_SUFFIX((
      async_initiate<WaitToken, void (zasio::error_code)>(
          declval<initiate_async_wait>(), token)))
  {
    return async_initiate<WaitToken, void (zasio::error_code)>(
        initiate_async_wait(this), token);
  }

private:
  // Disallow copying and assignment.
  basic_object_handle(const basic_object_handle&) ASIO_DELETED;
  basic_object_handle& operator=(const basic_object_handle&) ASIO_DELETED;

  class initiate_async_wait
  {
  public:
    typedef Executor executor_type;

    explicit initiate_async_wait(basic_object_handle* self)
      : self_(self)
    {
    }

    const executor_type& get_executor() const ASIO_NOEXCEPT
    {
      return self_->get_executor();
    }

    template <typename WaitHandler>
    void operator()(ASIO_MOVE_ARG(WaitHandler) handler) const
    {
      // If you get an error on the following line it means that your handler
      // does not meet the documented type requirements for a WaitHandler.
      ASIO_WAIT_HANDLER_CHECK(WaitHandler, handler) type_check;

      detail::non_const_lvalue<WaitHandler> handler2(handler);
      self_->impl_.get_service().async_wait(
          self_->impl_.get_implementation(),
          handler2.value, self_->impl_.get_executor());
    }

  private:
    basic_object_handle* self_;
  };

  zasio::detail::io_object_impl<
    zasio::detail::win_object_handle_service, Executor> impl_;
};

} // namespace windows
} // namespace zasio

#include "asio/detail/pop_options.hpp"

#endif // defined(ASIO_HAS_WINDOWS_OBJECT_HANDLE)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // ASIO_WINDOWS_BASIC_OBJECT_HANDLE_HPP