#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast; // Boost.Beast
namespace http = beast::http;   // HTTP from Beast
namespace net = boost::asio;    // Boost.Asio
using tcp = net::ip::tcp;       // TCP from Asio
using namespace std::literals;  // For suffixes like "ms"

// Asynchronous function to perform the HTTP GET request
net::awaitable<void> async_http_get(const std::string& host, const std::string& port) {
    try {
        auto executor = co_await net::this_coro::executor;
        tcp::resolver resolver(executor);
        tcp::socket socket(executor);

        // Resolve the host and connect
        auto results = co_await resolver.async_resolve(host, port, net::use_awaitable);
        co_await net::async_connect(socket, results, net::use_awaitable);

        // Set up HTTP GET request
        http::request<http::string_body> req{http::verb::get, "/", 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the request
        co_await http::async_write(socket, req, net::use_awaitable);

        // Buffer for reading
        beast::flat_buffer buffer;

        // Container for the response
        http::response<http::dynamic_body> res;

        // Receive the response
        co_await http::async_read(socket, buffer, res, net::use_awaitable);

        // Print the response
        std::cout << res << std::endl;

        // Gracefully close the connection
        beast::error_code ec;
        socket.shutdown(tcp::socket::shutdown_both, ec);

        if (ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        net::io_context ioc;

        // Run the coroutine
        net::co_spawn(ioc, async_http_get("example.com", "80"), net::detached);

        // Run the I/O context
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
