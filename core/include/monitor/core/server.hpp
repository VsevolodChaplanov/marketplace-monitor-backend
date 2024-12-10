#ifndef MARKETPLACEMONITOR_SERVER_HPP
#define MARKETPLACEMONITOR_SERVER_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/fields.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <concepts>
#include <exception>
#include <monitor/core/exports/exports.hpp>
#include <spdlog/spdlog.h>
#include <type_traits>

namespace mo::core {

    namespace net = boost::asio;
    namespace beast = boost::beast;
    namespace http = beast::http;

    using tcp_acceptor = net::use_awaitable_t<>::as_default_on_t<net::ip::tcp::acceptor>;
    using tcp_socket = net::use_awaitable_t<>::as_default_on_t<net::ip::tcp::socket>;
    using tcp_stream = net::use_awaitable_t<>::as_default_on_t<beast::tcp_stream>;

    namespace detail {
        template<typename R, typename F, typename... Args>
        concept iregular_invocable_r =
            std::regular_invocable<F, Args...> && std::convertible_to<std::invoke_result_t<F, Args...>, R>;
    }

    template<typename handler, typename request_body, typename request_allocator>
    concept irequest_handler =
        std::copyable<handler> &&
        detail::iregular_invocable_r<net::awaitable<http::message_generator>, handler,
                                     http::request<request_body, http::basic_fields<request_allocator>>>;

    namespace detail {
        template<typename request_body, typename request_allocator,
                 irequest_handler<request_body, request_allocator> request_handler>
        auto session(tcp_stream stream, request_handler handler) -> net::awaitable<void> {
            auto session_strand = net::make_strand(co_await net::this_coro::executor);
            beast::flat_buffer buffer;

            for (http::request<http::string_body> request;;) {
                co_await http::async_read(stream, buffer, request);

                http::message_generator message_generator = co_await handler(std::move(request));
                bool keep_alive = message_generator.keep_alive();

                co_await beast::async_write(stream, std::move(message_generator));

                if (!keep_alive) {
                    break;
                }
            }
            co_return;
        }

        inline void handle_exception(std::exception_ptr exception = std::current_exception()) {
            try {
                std::rethrow_exception(std::move(exception));
            } catch (const std::exception& ex) {
                spdlog::error(ex.what());
            }
        }
    } // namespace detail

    template<typename request_body, typename request_allocator,
             irequest_handler<request_body, request_allocator> request_handler>
    auto listener(net::ip::tcp::endpoint endpoint, request_handler handler) -> net::awaitable<void> {
        auto executor = co_await net::this_coro::executor;
        auto acceptor = tcp_acceptor(executor, std::move(endpoint));

        // for(;auto&& socket = co_await acceptor.async_accept();)
        for (;;) {
            tcp_socket&& socket = co_await acceptor.async_accept();
            net::co_spawn(boost::asio::bind_executor(executor, &detail::handle_exception),
                          detail::session(std::move(socket), handler));
        }

        co_return;
    }
} // namespace mo::core

#endif // MARKETPLACEMONITOR_SERVER_HPP