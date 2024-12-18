#include <boost/asio/use_awaitable.hpp>
#include <monitor/core/isku_repository.hpp>

namespace mo::core {

    auto create_sku_table(sql::pooled_connection& connection) -> net::awaitable<void> {
        constexpr auto statement =
            R"(CREATE TABLE IF NOT EXISTS sku_model (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,       -- Unique identifier
sku BIGINT UNSIGNED NOT NULL UNIQUE,  -- SKU must be unique
url TEXT NOT NULL,           -- URL of the SKU
price DOUBLE NOT NULL,       -- Price of the SKU
CHECK (price >= 0)           -- Ensure price is non-negative
);)";

        boost::mysql::results result;
        co_await connection->async_execute(statement, result, net::use_awaitable);
    }

    auto get_sku_model(sql::pooled_connection& connection, id_t id) -> net::awaitable<sql_sku_model_t> {
        auto statement =
            co_await connection->async_prepare_statement(R"(SELECT * from sku_model WHERE id = ?)", net::use_awaitable);

        sql_sku_model_t result;

        co_await connection->async_execute(statement.bind(id), result, net::use_awaitable);

        co_await connection->async_close_statement(statement, net::use_awaitable);

        co_return result;
    }

    auto insert_sku_model(sql::pooled_connection& connection, sku_model model) -> net::awaitable<id_t> {
        auto statement = co_await connection->async_prepare_statement(
            R"(INSERT INFO sku_model (sku, url, price) VALUES (?, ?, ?)
               ON DUPLICATE KEY UPDATE sku = ?, url = ?, price = ?
            )",
            net::use_awaitable);

        sql_sku_model_t result;

        co_await connection->async_execute(statement.bind(model.sku, model.url, model.price), result,
                                           net::use_awaitable);

        co_await connection->async_close_statement(statement, net::use_awaitable);

        co_return result.last_insert_id();
    }

    auto update_price_for_sku_model(sql::pooled_connection& connection, std::uint64_t sku,
                                    double price) -> net::awaitable<sql_sku_model_t> {
        auto statement = co_await connection->async_prepare_statement(R"(UPDATE sku_model SET price = ? WHERE id = ?)",
                                                                      net::use_awaitable);

        sql_sku_model_t result;

        co_await connection->async_execute(statement.bind(sku, price), result, net::use_awaitable);

        co_await connection->async_close_statement(statement, net::use_awaitable);

        co_return result;
    }
} // namespace mo::core