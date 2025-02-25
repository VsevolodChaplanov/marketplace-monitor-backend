module;

#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/static_results.hpp>
#include <cassert>
#include <repository/exports/exports.hpp>

export module repository:items;

import domain;

namespace repository::items {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    export struct REPOSITORY_EXPORT CORE_EXPORT items_repository {
        explicit sku_repository(std::shared_ptr<sql::connection_pool> pool);

        auto CORE_EXPORT create_table() -> net::awaitable<void>;

        auto CORE_EXPORT get(models::id_t model_id) -> net::awaitable<models::sku_model_sql_result_t>;

        auto CORE_EXPORT insert(models::sku_model model) -> net::awaitable<models::id_t>;

        auto CORE_EXPORT update(std::uint64_t sku, double price) -> net::awaitable<models::sku_model_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };
} // namespace repository::items
