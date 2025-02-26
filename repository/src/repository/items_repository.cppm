module;

#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/cancellation_type.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <boost/asio/impl/any_completion_executor.ipp>
#include <boost/asio/impl/any_io_executor.ipp>
#include <boost/asio/impl/awaitable.hpp>
#include <boost/asio/impl/cancellation_signal.ipp>
#include <boost/mp11/detail/mp_with_index.hpp>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/constant_string_view.hpp>
#include <boost/mysql/detail/writable_field_traits.hpp>
#include <boost/mysql/pfr.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/static_results.hpp>
#include <boost/mysql/with_params.hpp>
#include <boost/pfr/detail/core17.hpp>
#include <cassert>
#include <coroutine>
#include <cstdint>
#include <exception>
#include <memory>
#include <repository/exports/exports.hpp>
#include <string>
#include <tuple>
#include <utility>

export module repository.items;

import domain.item;

namespace repository::items {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    using item = domain::item_model;
    using id_t = domain::id_t;

    using item_sql_result_t = sql::static_results<sql::pfr_by_name<item>>;

    export struct REPOSITORY_EXPORT items_repository {
        explicit items_repository(std::shared_ptr<sql::connection_pool> pool);

        auto create_table() -> net::awaitable<void>;

        auto get(id_t model_id) -> net::awaitable<item>;

        auto insert(item model) -> net::awaitable<id_t>;

        auto update(std::uint64_t sku, double price) -> net::awaitable<item>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

} // namespace repository::items

module :private;

namespace repository::items {

    items_repository::items_repository(std::shared_ptr<sql::connection_pool> pool) : m_pool{std::move(pool)} {}

    auto items_repository::create_table() -> net::awaitable<void> {
        auto&& connection = co_await m_pool->async_get_connection();

        constexpr auto statement = R"(CREATE TABLE IF NOT EXISTS sku_model (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,                              -- Unique identifier
sku BIGINT UNSIGNED NOT NULL UNIQUE,                                        -- SKU must be unique
url TEXT NOT NULL,                                                          -- URL of the SKU
price DECIMAL(10, 2) NOT NULL,                                              -- Price of the SKU
created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, 
updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
CHECK (price >= 0)                                                          -- Ensure price is non-negative
);)";

        sql::results result;
        co_await connection->async_execute(statement, result);
    }

    auto items_repository::get(id_t model_id) -> net::awaitable<item> {
        auto&& connection = co_await m_pool->async_get_connection();

        item_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("SELECT id, sku, url, price FROM sku_model WHERE id = {}", model_id), result);

        connection.return_without_reset();

        // todo: convert
        assert(false && "todo: convert result");
        co_return item{};
    }

    auto items_repository::insert(item model) -> net::awaitable<id_t> {
        auto&& connection = co_await m_pool->async_get_connection();
        item_sql_result_t result;
        co_await connection->async_execute(
            sql::with_params("INSERT INTO sku_model (sku, url, price) VALUES ({}, {}, {})", model.sku, model.url,
                             model.price),
            result);

        co_return result.last_insert_id();
    }

    auto items_repository::update(std::uint64_t sku, double price) -> net::awaitable<item> {
        auto&& connection = co_await m_pool->async_get_connection();
        item_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("UPDATE sku_model SET price = {} WHERE sku = {}", price, sku), result);

        // todo: convert
        assert(false && "todo: convert result");
        co_return item{};
    }
} // namespace repository::items