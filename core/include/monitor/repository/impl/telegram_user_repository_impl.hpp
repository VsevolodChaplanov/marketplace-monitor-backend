#ifndef MONITOR_REPOSITORY_IMPL_TELEGRAM_USER_REPOSITORY_IMPL_HPP
#define MONITOR_REPOSITORY_IMPL_TELEGRAM_USER_REPOSITORY_IMPL_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/mysql.hpp>
#include <memory>
#include <monitor/core/exports/exports.hpp>
#include <monitor/models/user_model.hpp>

namespace mo::repository::impl {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    struct CORE_EXPORT telegram_user_repository_impl {

        explicit telegram_user_repository_impl(std::shared_ptr<sql::connection_pool> pool);

        [[nodiscard("co_await")]] auto create_table() const -> net::awaitable<void>;

        [[nodiscard("co_await")]] auto
        get(models::id_t user_id) const -> net::awaitable<models::telegram_user_sql_result_t>;

    private:
        std::shared_ptr<sql::connection_pool> m_pool;
    };

} // namespace mo::repository::impl

#endif // MONITOR_REPOSITORY_IMPL_TELEGRAM_USER_REPOSITORY_IMPL_HPP