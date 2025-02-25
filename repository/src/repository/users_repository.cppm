module repository:users;

import domain;

namespace repository::users {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

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

    telegram_user_repository_impl::telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool)
        : m_pool(std::move(pool)) {}

    /*************  ✨ Codeium Command ⭐  *************/
    /******  1c82dcef-9672-4bf2-acde-eda413a70d9d  *******/
    auto telegram_user_repository_impl::create_table() const -> net::awaitable<void> {
        auto&& connection = co_await m_pool->async_get_connection();

        constexpr auto statement = R"(CREATE TABLE IF NOT EXISTS telegram_user_model (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
telegram_id BIGINT UNSIGNED NOT NULL,
user_fk BIGINT UNSIGNED NOT NULL,
created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, 
updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);)";

        boost::mysql::results result;
        co_await connection->async_execute(statement, result);
    }

    auto telegram_user_repository_impl::get(models::id_t user_id) const
        -> net::awaitable<models::telegram_user_sql_result_t> {
        auto&& connection = co_await m_pool->async_get_connection();

        models::telegram_user_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("SELECT id, telegram_id, user_fk FROM telegram_user_model WHERE user_fk = {}",
                                      user_id),
            result);

        connection.return_without_reset();

        co_return result;
    }
} // namespace repository::users