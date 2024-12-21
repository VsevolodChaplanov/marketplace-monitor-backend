#include <monitor/repository/impl/user_repository_impl.hpp>

namespace mo::repository::impl {

    user_repository_impl::user_repository_impl(std::shared_ptr<sql::connection_pool> pool) : m_pool(std::move(pool)) {}

    auto user_repository_impl::create_table() const -> net::awaitable<void> {
        auto&& connection = co_await m_pool->async_get_connection();

        constexpr auto statement = R"(CREATE TABLE IF NOT EXISTS user_model (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
login VARCHAR(255) UNIQUE NOT NULL,
password_hash VARCHAR(255) NOT NULL,
created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, 
updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
is_active BOOLEAN DEFAULT TRUE,
is_accepted BOOLEAN DEFAULT FALSE
);)";

        boost::mysql::results result;
        co_await connection->async_execute(statement, result);
    }

    auto user_repository_impl::get(models::id_t user_id) const -> net::awaitable<models::user_sql_result_t> {
        auto&& connection = co_await m_pool->async_get_connection();

        models::user_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("SELECT id FROM user_model WHERE id = {}", user_id), result);

        connection.return_without_reset();

        co_return result;
    }
} // namespace mo::repository::impl