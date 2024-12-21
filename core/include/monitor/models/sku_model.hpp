#ifndef MONITOR_CORE_MODELS_HPP
#define MONITOR_CORE_MODELS_HPP

#include <boost/mysql/datetime.hpp>
#include <boost/mysql/pfr.hpp>
#include <boost/mysql/static_results.hpp>
#include <cstdint>
#include <monitor/core/exports/exports.hpp>
#include <monitor/models/common.hpp>

namespace mo::models {
    struct CORE_EXPORT sku_model final {
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

    using sku_model_sql_result_t = boost::mysql::static_results<boost::mysql::pfr_by_name<sku_model>>;
} // namespace mo::models

#endif // MONITOR_CORE_MODELS_HPP