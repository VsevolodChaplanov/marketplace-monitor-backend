module;

#include <boost/mysql/datetime.hpp>
#include <cstdint>
#include <string>

export module domain:item;

namespace domain {
    export using datetime = boost::mysql::datetime;
    export using id_t = std::uint64_t;

    export struct sku_model final {
        id_t id{0};
        std::uint64_t sku{0};
        std::string url;
        double price;

        datetime created_at{datetime::now()};
        datetime updated_at{datetime::now()};

        constexpr friend auto operator<=>(const sku_model& self, const sku_model& other) noexcept {
            return self.id <=> other.id;
        }
    };
} // namespace domain
