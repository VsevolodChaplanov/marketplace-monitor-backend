module;

#include <boost/asio/awaitable.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/pfr.hpp>
#include <boost/mysql/static_results.hpp>
#include <boost/mysql/with_params.hpp>
#include <cassert>
#include <repository/exports/exports.hpp>
#include <utility>

export module repository.users;

export import domain.user;

namespace repository::users {

    namespace net = boost::asio;
    namespace sql = boost::mysql;

    using id_t = domain::id_t;
    using user = domain::user_model;
    using tg_user = domain::telegram_user_model;

    using user_sql_result_t = sql::static_results<sql::pfr_by_name<user>>;
    using tg_user_sql_result_t = sql::static_results<sql::pfr_by_name<tg_user>>;

    struct user_repository_impl {

        explicit user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto get(id_t user_id) const -> net::awaitable<user>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    struct telegram_user_repository_impl {

        explicit telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto get(id_t user_id) const -> net::awaitable<tg_user>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    export struct REPOSITORY_EXPORT iuser_repository {
    private:
        user_repository_impl m_user_repository;
        telegram_user_repository_impl m_telegram_user_repository;

    public:
        explicit iuser_repository(std::shared_ptr<sql::connection_pool> pool)
            : m_user_repository{pool}, m_telegram_user_repository{pool} {}
    };
} // namespace repository::users

module :private;

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

    auto user_repository_impl::get(id_t user_id) const -> net::awaitable<user> {
        auto&& connection = co_await m_pool->async_get_connection();

        user_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("SELECT id FROM user_model WHERE id = {}", user_id), result);

        connection.return_without_reset();

        // todo: convert
        assert(false && "todo: convert result");
        co_return user{};
    }

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

    auto telegram_user_repository_impl::get(id_t user_id) const -> net::awaitable<tg_user> {
        auto&& connection = co_await m_pool->async_get_connection();

        tg_user_sql_result_t result;

        co_await connection->async_execute(
            boost::mysql::with_params("SELECT id, telegram_id, user_fk FROM telegram_user_model WHERE user_fk = {}",
                                      user_id),
            result);

        connection.return_without_reset();

        // todo: convert
        assert(false && "todo: convert result");
        co_return tg_user{};
    }
} // namespace repository::users