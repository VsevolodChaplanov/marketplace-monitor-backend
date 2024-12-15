#ifndef MONITOR_CORE_PROGRAM_OPTIONS_HPP
#define MONITOR_CORE_PROGRAM_OPTIONS_HPP

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <monitor/core/exports/exports.hpp>
#include <span>

namespace mo::core {

    struct CORE_EXPORT connection_data final {
        std::string host;
        std::string database;
        std::string username;
        std::string password;
        unsigned int port;
    };

    void CORE_EXPORT add_connection_options(boost::program_options::options_description& options);

    auto CORE_EXPORT parse_command_line(std::span<const char* const> cmd_args,
                                        const boost::program_options::options_description& options)
        -> boost::program_options::variables_map;

    auto CORE_EXPORT parse_connection_data(const boost::program_options::variables_map& vm) -> connection_data;
} // namespace mo::core

#endif // MONITOR_CORE_PROGRAM_OPTIONS_HPP