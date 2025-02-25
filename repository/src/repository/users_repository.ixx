module;

#include <boost/asio/awaitable.hpp>
#include <boost/mysql.hpp>
#include <repository/exports/exports.hpp>

export module repository:users;

import domain;

namespace repository::users {

    namespace net = boost::asio;
    namespace sql = boost::mysql;

    struct user_repository_impl {

        explicit user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto get(models::id_t user_id) const -> net::awaitable<models::user_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    struct telegram_user_repository_impl {

        explicit telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto get(models::id_t user_id) const
            -> net::awaitable<models::telegram_user_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    export struct REPOSITORY_EXPORT iuser_repository {
    private:
        impl::user_repository_impl m_user_repository;
        impl::telegram_user_repository_impl m_telegram_user_repository;

    public:
        explicit iuser_repository(std::shared_ptr<sql::connection_pool> pool)
            : m_user_repository{pool}, m_telegram_user_repository{pool} {}
    };
} // namespace repository::users
