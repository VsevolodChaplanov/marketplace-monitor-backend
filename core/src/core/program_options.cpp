#include <monitor/core/program_options.hpp>

namespace mo::core {

    auto parse_connection_data(const boost::program_options::variables_map& vm) -> connection_data {
        connection_data config;
        config.host = vm["host"].as<std::string>();
        config.port = vm["port"].as<int>();
        config.database = vm["database"].as<std::string>();
        config.username = vm["username"].as<std::string>();
        config.password = vm["password"].as<std::string>();

        return config;
    }

    auto parse_command_line(std::span<const char* const> cmd_args,
                            const boost::program_options::options_description& options)
        -> boost::program_options::variables_map {
        namespace po = boost::program_options;
        po::variables_map vm;
        po::store(po::parse_command_line(static_cast<int>(cmd_args.size()), cmd_args.data(), options), vm);
        po::notify(vm);
        return vm;
    }

    void add_connection_options(boost::program_options::options_description& options) {
        namespace po = boost::program_options;

        constexpr auto default_port = 3306;

        boost::program_options::options_description desc("Database connection options");

        // clang-format off
        desc.add_options()
            ("host,H", po::value<std::string>()->default_value("127.0.0.1"), "Database host")
            ("port,P", po::value<int>()->default_value(default_port), "Database port")
            ("database,d", po::value<std::string>()->default_value("mysql"), "Database name")
            ("username,u", po::value<std::string>()->default_value("mysql"), "Database username")
            ("password,p", po::value<std::string>()->default_value("mysql"), "Database password");
        // clang-format on

        options.add(desc);
    }
} // namespace mo::core