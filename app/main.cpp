#include <boost/asio/as_tuple.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/version.hpp>

#include <certificates/certificates.hpp>

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>

#if defined(BOOST_ASIO_HAS_CO_AWAIT)

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

//------------------------------------------------------------------------------

// Performs an HTTP GET and prints the response
net::awaitable<void> do_session(std::string host, std::string port, std::string target, int version,
                                ssl::context& ctx) {
    auto executor = co_await net::this_coro::executor;
    auto resolver = net::ip::tcp::resolver{executor};
    auto stream = ssl::stream<beast::tcp_stream>{executor, ctx};

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
        throw boost::system::system_error(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());
    }

    // Look up the domain name
    auto const results = co_await resolver.async_resolve(host, port);

    // Set the timeout.
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    co_await beast::get_lowest_layer(stream).async_connect(results);

    // Set the timeout.
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

    // Perform the SSL handshake
    co_await stream.async_handshake(ssl::stream_base::client);

    // Set the timeout.
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    for (const auto i : std::views::iota(0, 1000)) { // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::get, target, version};
        std::cout << target << std::endl;
        req.keep_alive(true);
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        co_await http::async_write(stream, req);
        beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Receive the HTTP response
        co_await http::async_read(stream, buffer, res);
        beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

        auto&& result = res.result();
        if (result == http::status::ok) {
            std::cout << res << std::endl;
            break;
        }

        // Write the message to standard out
        // std::cout << res << std::endl;

        target = res[http::field::location];
    }

    // Set the timeout.
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

    // Gracefully close the stream - do not threat every error as an exception!
    auto [ec] = co_await stream.async_shutdown(net::as_tuple);

    if (ec && ec != net::ssl::error::stream_truncated) {
        throw boost::system::system_error(ec, "shutdown");
    }
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
    try {
        const auto* const host = "market.yandex.ru";
        const auto* const port = "443";
        const auto* const target =
            "/product--novaia-stantsiia-mini-umnaia-kolonka-s-alisoi/"
            "1423994419?sku=101429558735&uniqueId=4852853&do-waremd5=y3NNVQLtEmODl35hBxVX-g&resale_goods=resale_new";
        // "/product/tualetnaya-bumaga-kix-3-sloya-12-rulonov-1614138493/?avtc=1&avte=4&avts=1733173513";
        auto const version = 11;

        // The io_context is required for all I/O
        net::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx{ssl::context::tlsv12_client};

        // This holds the root certificate used for verification
        load_root_certificates(ctx);

        // Verify the remote server's certificate
        ctx.set_verify_mode(ssl::verify_peer);

        // Launch the asynchronous operation
        net::co_spawn(ioc, do_session(host, port, target, version, ctx),
                      // If the awaitable exists with an exception, it gets delivered here
                      // as `e`. This can happen for regular errors, such as connection
                      // drops.
                      [](std::exception_ptr e) {
                          if (e)
                              std::rethrow_exception(e);
                      });

        // Run the I/O service. The call will return when
        // the get operation is complete.
        ioc.run();
    } catch (std::exception const & e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#else

int main(int, char*[]) {
    std::printf("awaitables require C++20\n");
    return EXIT_FAILURE;
}

#endif