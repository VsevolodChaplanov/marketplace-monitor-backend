#ifndef MARKETPLACEMONITOR_REPOSITORY_HPP
#define MARKETPLACEMONITOR_REPOSITORY_HPP

#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <cassert>
#include <monitor/core/exports/exports.hpp>
#include <monitor/core/models.hpp>

namespace mo::core {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    struct CORE_EXPORT isku_repository {};
} // namespace mo::core

#endif // MARKETPLACEMONITOR_REPOSITORY_HPP