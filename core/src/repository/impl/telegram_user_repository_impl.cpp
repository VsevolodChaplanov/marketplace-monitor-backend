#include <monitor/repository/impl/telegram_user_repository_impl.hpp>

namespace mo::repository::impl {

    telegram_user_repository_impl::telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool)
        : m_pool(std::move(pool)) {}

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
} // namespace mo::repository::impl