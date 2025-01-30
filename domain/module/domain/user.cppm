module;

#include <boost/mysql/datetime.hpp>
#include <boost/mysql/pfr.hpp>

export module domain:user;

namespace domain {

    export using datetime = boost::mysql::datetime;
    export using id_t = std::uint64_t;

    export struct user_model final {
        id_t id;

        std::string login;
        std::string password_hash;

        datetime created_at;
        datetime updated_at;

        bool is_active{true};
        bool is_accepted{false};
    };

    export struct telegram_user_model final {
        id_t id;

        id_t telegram_id;
        id_t user_fk;

        datetime created_at;
        datetime updated_at;
    };
} // namespace domain