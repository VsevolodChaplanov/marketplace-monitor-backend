#ifndef MONITOR_MODELS_COMMON_HPP
#define MONITOR_MODELS_COMMON_HPP

#include <boost/mysql/datetime.hpp>

namespace mo::models {
    using id_t = std::uint64_t;
    using datetime = boost::mysql::datetime;
} // namespace mo::models

#endif // MONITOR_MODELS_COMMON_HPP