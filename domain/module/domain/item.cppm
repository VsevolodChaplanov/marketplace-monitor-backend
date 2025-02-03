module;

#include <chrono>
#include <cstdint>
#include <string>

export module domain:item;

namespace domain {
    using clock = std::chrono::system_clock;

    export using time_point_t = clock::time_point;
    export using id_t = std::uint64_t;

    export struct sku_model final {
        id_t id{0};
        std::uint64_t sku{0};
        std::string url;
        double price;

        time_point_t created_at{clock::now()};
        time_point_t updated_at{clock::now()};

        constexpr friend auto operator==(const sku_model& self, const sku_model& other) noexcept {
            return self.id == other.id;
        }

        constexpr friend auto operator!=(const sku_model& self, const sku_model& other) noexcept {
            return !(self == other);
        }
    };
} // namespace domain
