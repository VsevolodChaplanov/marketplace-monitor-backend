#ifndef MONITOR_REPOSITORY_IUSER_REPOSITORY_HPP
#define MONITOR_REPOSITORY_IUSER_REPOSITORY_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/mysql.hpp>
#include <memory>
#include <monitor/core/exports/exports.hpp>
#include <monitor/models/user_model.hpp>
#include <monitor/repository/impl/telegram_user_repository_impl.hpp>
#include <monitor/repository/impl/user_repository_impl.hpp>

namespace mo::repository {

    namespace net = boost::asio;
    namespace sql = boost::mysql;

    struct CORE_EXPORT iuser_repository {
    private:
        impl::user_repository_impl m_user_repository;
        impl::telegram_user_repository_impl m_telegram_user_repository;

    public:
        explicit iuser_repository(std::shared_ptr<sql::connection_pool> pool)
            : m_user_repository{pool}, m_telegram_user_repository{pool} {}
    };
} // namespace mo::repository

#endif // MONITOR_REPOSITORY_IUSER_REPOSITORY_HPP