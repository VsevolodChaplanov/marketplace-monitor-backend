#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/any_address.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/diagnostics.hpp>
#include <boost/mysql/error_with_diagnostics.hpp>
#include <boost/mysql/pool_params.hpp>
#include <boost/mysql/with_diagnostics.hpp>
#include <boost/program_options/options_description.hpp>
#include <cstdlib>
#include <fmt/base.h>
#include <iostream>
#include <memory>
#include <monitor/core/program_options.hpp>
#include <monitor/models/sku_model.hpp>
#include <monitor/repository/isku_repository.hpp>
#include <ranges>
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

    boost::asio::io_context ctx{static_cast<int>(std::thread::hardware_concurrency())};
    boost::asio::thread_pool th_pool{std::thread::hardware_concurrency()};

    boost::mysql::pool_params parameters{
        boost::mysql::host_and_port{config.host, static_cast<unsigned short>(config.port)}, config.username,
        config.password, config.database};
    parameters.thread_safe = true;
    parameters.initial_size = 2;

    const auto connection_pool = std::make_shared<boost::mysql::connection_pool>(ctx, std::move(parameters));

    // A signal_set allows us to intercept SIGINT and SIGTERM and
    // exit gracefully
    boost::asio::signal_set signals{ctx.get_executor(), SIGINT, SIGTERM};

    connection_pool->async_run(boost::asio::detached);

    auto strand = boost::asio::make_strand(ctx.get_executor());

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    signals.async_wait([connection_pool, &th_pool, &ctx](boost::system::error_code, int signal) {
        fmt::println("capture {}", signal);

        // Cancel the pool. This will cause async_run to complete.
        connection_pool->cancel();
        fmt::println("cancelled thread pool", signal);

        // Stop the execution context. This will cause main to exit
        ctx.stop();
        fmt::println("stop io_context", signal);

        th_pool.stop();
        fmt::println("stop thread_pool", signal);
    });

    boost::asio::io_context::work guard{ctx};

    for (const auto& i : std::views::iota(unsigned{0}, std::thread::hardware_concurrency())) {
        boost::asio::post(th_pool, [&ctx] { ctx.run(); });
    }

    th_pool.attach();

    fmt::println("joining threads");

    th_pool.join();

    fmt::println("joined threads");
}
