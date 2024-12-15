#include <boost/program_options/options_description.hpp>
#include <cstdlib>
#include <fmt/base.h>
#include <iostream>
#include <monitor/core/program_options.hpp>
#include <string_view>

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
}
