#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql/any_address.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/pool_params.hpp>
#include <boost/program_options/options_description.hpp>
#include <cstdlib>
#include <fmt/base.h>
#include <iostream>
#include <memory>
#include <monitor/core/program_options.hpp>
#include <string_view>
#include <thread>

int main(int argc, const char* const argv[]) {
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Show help message");

    mo::core::add_connection_options(desc);

    const auto vm = mo::core::parse_command_line(std::span{argv, static_cast<std::size_t>(argc)}, desc);

    if (constexpr std::string_view help = "help"; vm.contains(help.data())) {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    const auto config = mo::core::parse_connection_data(vm);

    boost::asio::thread_pool th_pool{std::thread::hardware_concurrency()};

    boost::mysql::pool_params parameters{
        boost::mysql::host_and_port{config.host, static_cast<unsigned short>(config.port)}, config.username,
        config.password, config.database};

    const auto connection_pool = std::make_shared<boost::mysql::connection_pool>(th_pool, std::move(parameters));

    // A signal_set allows us to intercept SIGINT and SIGTERM and
    // exit gracefully
    boost::asio::signal_set signals{th_pool.get_executor(), SIGINT, SIGTERM};

    connection_pool->async_run(boost::asio::detached);

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    signals.async_wait([connection_pool, &th_pool](boost::system::error_code, int) {
        // Cancel the pool. This will cause async_run to complete.
        connection_pool->cancel();

        // Stop the execution context. This will cause main to exit
        th_pool.stop();
    });

    boost::asio::co_spawn(
        th_pool.get_executor(),
        [connection_pool]() -> boost::asio::awaitable<void> {
            auto conn = co_await connection_pool->async_get_connection(boost::asio::use_awaitable);

            const char* sql = "SELECT 'Hello world!'";
            boost::mysql::results result;
            co_await conn->async_execute(sql, result, boost::asio::use_awaitable);

            std::cout << result.rows().at(0).at(0) << std::endl;
        },
        boost::asio::detached);

    // Wait until all threads have exited
    th_pool.join();
}
