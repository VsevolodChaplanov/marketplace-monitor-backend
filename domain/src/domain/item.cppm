module;

#include <chrono>
#include <cstdint>
#include <string>

#include <domain/exports/exports.hpp>

export module domain.item;

namespace domain {
    using clock = std::chrono::system_clock;

    export using time_point_t = clock::time_point;
    export using id_t = std::uint64_t;
    export using sku_t = std::uint64_t;

    export struct DOMAIN_EXPORT item_model final {
        id_t id{0};
        sku_t sku{0};
        std::string url;
        double price;

        constexpr friend auto operator==(const item_model& self, const item_model& other) noexcept {
            return self.id == other.id;
        }

        constexpr friend auto operator!=(const item_model& self, const item_model& other) noexcept {
            return !(self == other);
        }
    };
} // namespace domain
