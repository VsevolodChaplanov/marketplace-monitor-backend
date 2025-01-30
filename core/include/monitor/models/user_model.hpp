#ifndef MONITOR_MODELS_USER_MODEL_HPP
#define MONITOR_MODELS_USER_MODEL_HPP

#include <boost/mysql/pfr.hpp>
#include <boost/mysql/static_results.hpp>
#include <monitor/core/exports/exports.hpp>
#include <monitor/models/common.hpp>

namespace mo::models {

    struct CORE_EXPORT user_model final {
        id_t id;

        std::string login;
        std::string password_hash;

        datetime created_at;
        datetime updated_at;

        bool is_active{true};
        bool is_accepted{false};
    };

    struct CORE_EXPORT telegram_user_model final {
        id_t id;

        id_t telegram_id;
        id_t user_fk;

        datetime created_at;
        datetime updated_at;
    };

    using user_sql_result_t = boost::mysql::static_results<boost::mysql::pfr_by_name<user_model>>;
    using telegram_user_sql_result_t = boost::mysql::static_results<boost::mysql::pfr_by_name<telegram_user_model>>;
} // namespace mo::models

#endif // MONITOR_MODELS_USER_MODEL_HPP