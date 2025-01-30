module;

#include <span>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/parsers.hpp>

export module app:program_options;

namespace program_options {
    namespace po = boost::program_options;

    using descriptions = po::options_description;

    auto application_options() -> descriptions {
        descriptions desc("CLI interface options");

        constexpr auto database_description = [] {
            constexpr auto default_port = 3306;
            descriptions desc("Database connection options");

            // clang-format off
            desc.add_options()
                ("port,P", po::value<int>()->default_value(default_port), "Database port")
                ("host,H", po::value<std::string>()->default_value("127.0.0.1"), "Database host")
                ("database,d", po::value<std::string>()->default_value("mysql"), "Database name")
                ("username,u", po::value<std::string>()->default_value("mysql"), "Database username")
                ("password,p", po::value<std::string>()->default_value("mysql"), "Database password");
            // clang-format on

            return desc;
        };

        desc.add(database_description());

        return desc;
    }

    export using options = po::variables_map;

    export auto parse_command_line(std::span<const char* const> cmd_args) -> options {
        const auto options = application_options();
        po::variables_map map;
        po::store(po::parse_command_line(static_cast<int>(cmd_args.size()), cmd_args.data(), options), map);
        po::notify(map);
        return map;
    }
} // namespace program_options
