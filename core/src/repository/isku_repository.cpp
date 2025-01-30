#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql.hpp>
#include <monitor/models/sku_model.hpp>
#include <monitor/repository/isku_repository.hpp>

namespace mo::repository {
    auto CORE_EXPORT sku_repository::create_table() -> net::awaitable<void> {
        auto&& connection = co_await m_pool->async_get_connection();

        constexpr auto statement =
            R"(CREATE TABLE IF NOT EXISTS sku_model (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,                              -- Unique identifier
sku BIGINT UNSIGNED NOT NULL UNIQUE,                                        -- SKU must be unique
url TEXT NOT NULL,                                                          -- URL of the SKU
price DOUBLE NOT NULL,                                                      -- Price of the SKU
created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, 
updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
CHECK (price >= 0)                                                          -- Ensure price is non-negative
);)";

        boost::mysql::results result;
        co_await connection->async_execute(statement, result, net::use_awaitable);
    }

    auto CORE_EXPORT sku_repository::get(models::id_t model_id) -> net::awaitable<models::sku_model_sql_result_t> {
        auto&& connection = co_await m_pool->async_get_connection();

        models::sku_model_sql_result_t result;

        co_await connection->async_execute(boost::mysql::with_params("SELECT id, sku, url, price FROM sku_model"),
                                           result, //
                                           boost::mysql::with_diagnostics(boost::asio::use_awaitable));

        connection.return_without_reset();

        co_return result;
    }

    auto CORE_EXPORT sku_repository::insert(models::sku_model model) -> net::awaitable<models::id_t> {
        auto&& connection = co_await m_pool->async_get_connection();
        models::sku_model_sql_result_t result;
        co_await connection->async_execute(
            sql::with_params(R"(INSERT INTO sku_model (sku, url, price) VALUES ({0}, {1}, {2})
               ON DUPLICATE KEY UPDATE price = {2}?
            )",
                             model.sku, model.url, model.price),
            result, net::use_awaitable);

        connection.return_without_reset();

        co_return result.last_insert_id();
    }

    auto CORE_EXPORT sku_repository::update(std::uint64_t sku,
                                            double price) -> net::awaitable<models::sku_model_sql_result_t> {
        auto&& connection = co_await m_pool->async_get_connection();
        models::sku_model_sql_result_t result;

        co_await connection->async_execute(
            sql::with_params("UPDATE sku_model SET price = {0} WHERE id = {1}", sku, price), result,
            net::use_awaitable);

        connection.return_without_reset();

        co_return result;
    }
} // namespace mo::repository