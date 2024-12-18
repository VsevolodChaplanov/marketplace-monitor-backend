#ifndef MARKETPLACEMONITOR_REPOSITORY_HPP
#define MARKETPLACEMONITOR_REPOSITORY_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/static_results.hpp>
#include <cassert>
#include <monitor/core/exports/exports.hpp>
#include <monitor/core/models.hpp>

namespace mo::core {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    using sql_sku_model_t = sql::static_results<sql::pfr_by_position<sku_model>>;

    auto CORE_EXPORT create_sku_table(sql::pooled_connection& connection) -> net::awaitable<void>;

    auto CORE_EXPORT get_sku_model(sql::pooled_connection& connection, id_t id) -> net::awaitable<sql_sku_model_t>;

    auto CORE_EXPORT insert_sku_model(sql::pooled_connection& connection, sku_model model) -> net::awaitable<id_t>;

    auto CORE_EXPORT update_price_for_sku_model(sql::pooled_connection& connection, std::uint64_t sku,
                                                double price) -> net::awaitable<sql_sku_model_t>;
} // namespace mo::core

#endif // MARKETPLACEMONITOR_REPOSITORY_HPP