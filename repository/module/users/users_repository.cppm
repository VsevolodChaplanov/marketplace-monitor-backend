module;

#include <boost/asio/awaitable.hpp>
#include <boost/mysql.hpp>
#include <memory>
#include <monitor/core/exports/exports.hpp>
#include <monitor/models/user_model.hpp>
#include <monitor/repository/impl/telegram_user_repository_impl.hpp>
#include <monitor/repository/impl/user_repository_impl.hpp>

export module repository:users;

namespace repository::users {

    namespace net = boost::asio;
    namespace sql = boost::mysql;

    struct CORE_EXPORT user_repository_impl {

        explicit user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto get(models::id_t user_id) const -> net::awaitable<models::user_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    struct CORE_EXPORT telegram_user_repository_impl {

        explicit telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto
        get(models::id_t user_id) const -> net::awaitable<models::telegram_user_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

    export struct CORE_EXPORT iuser_repository {
    private:
        impl::user_repository_impl m_user_repository;
        impl::telegram_user_repository_impl m_telegram_user_repository;

    public:
        explicit iuser_repository(std::shared_ptr<sql::connection_pool> pool)
            : m_user_repository{pool}, m_telegram_user_repository{pool} {}
    };
} // namespace repository::users

#endif // MONITOR_REPOSITORY_IUSER_REPOSITORY_HPP