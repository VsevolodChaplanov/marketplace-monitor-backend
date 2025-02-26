module;

#include <cstdint>
#include <string>

#include <domain/exports/exports.hpp>

export module domain.user;

namespace domain {
    export using id_t = std::uint64_t;

    export struct DOMAIN_EXPORT user_model final {
        id_t id;

        std::string login;
        std::string password_hash;

        bool is_active{true};
        bool is_accepted{false};

        constexpr auto operator<=>(const user_model& other) const noexcept { return id <=> other.id; }
    };

    export struct DOMAIN_EXPORT telegram_user_model final {
        id_t id{0};

        id_t telegram_id{0};
        id_t user_fk{0};

        constexpr auto operator<=>(const user_model& other) const noexcept { return id <=> other.id; }
    };
} // namespace domain