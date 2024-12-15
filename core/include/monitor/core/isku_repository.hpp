#ifndef MARKETPLACEMONITOR_REPOSITORY_HPP
#define MARKETPLACEMONITOR_REPOSITORY_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/mysql.hpp>
#include <boost/mysql/any_connection.hpp>
#include <boost/mysql/connection_pool.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/static_results.hpp>
#include <cassert>
#include <monitor/core/exports/exports.hpp>
#include <monitor/core/models.hpp>

namespace mo::core {
    namespace net = boost::asio;
    namespace sql = boost::mysql;

    template<typename model> struct base_get {
        using result = sql::static_results<sql::pfr_by_name<model>>;
        using statement = sql::statement;

        auto get(sql::any_connection& connection, const statement& query,
                 id_t model_id) const -> net::awaitable<result> {
            result result;
            co_await connection.async_execute(query.bind(model_id), result, net::use_awaitable);
            co_return result;
        }
    };

    struct CORE_EXPORT base_sku_repository : private base_get<sku_model> {
        using result = typename base_get<sku_model>::result;

        net::awaitable<result> get(sql::any_connection& connection, id_t model_id) const {
            auto statement =
                co_await connection.async_prepare_statement("SELECT * FROM sku WHERE id = ?", net::use_awaitable);

            co_return co_await base_get<sku_model>::get(connection, statement, model_id);
        }
    };
} // namespace mo::core

#endif // MARKETPLACEMONITOR_REPOSITORY_HPP