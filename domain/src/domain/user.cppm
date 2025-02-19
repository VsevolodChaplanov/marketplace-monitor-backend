module;

#include <chrono>
#include <cstdint>
#include <string>

#include <domain/exports/exports.hpp>

export module domain:user;

namespace domain {

    using clock = std::chrono::system_clock;

    export using time_point_t = clock::time_point;
    export using id_t = std::uint64_t;

    export struct DOMAIN_EXPORT user_model final {
        id_t id;

        std::string login;
        std::string password_hash;

        time_point_t created_at{clock::now()};
        time_point_t updated_at{clock::now()};

        bool is_active{true};
        bool is_accepted{false};
    };

    export struct DOMAIN_EXPORT telegram_user_model final {
        id_t id{0};

        id_t telegram_id{0};
        id_t user_fk{0};

        time_point_t created_at{clock::now()};
        time_point_t updated_at{clock::now()};
    };
} // namespace domain