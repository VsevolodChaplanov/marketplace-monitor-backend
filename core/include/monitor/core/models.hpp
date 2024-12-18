#ifndef MONITOR_CORE_MODELS_HPP
#define MONITOR_CORE_MODELS_HPP

#include <boost/mysql/pfr.hpp>
#include <boost/mysql/static_results.hpp>
#include <cstdint>
#include <monitor/utility/strong_type.hpp>

namespace mo::core {

    using id_t = std::uint64_t;

    /**
     * @brief updating sku and url is useless - it looks like need to add instead of update
     *
     */
    struct update_sku_model {
        double price;
    };

    struct sku_model final {
        id_t id{0};
        std::uint64_t sku{0};
        std::string url;
        double price;

        constexpr friend auto operator<=>(const sku_model&, const sku_model&) noexcept = default;
    };

    using sku_model_sql_t = boost::mysql::pfr_by_name<sku_model>;
    using sku_model_sql_result_t = boost::mysql::static_results<sku_model_sql_t>;
} // namespace mo::core

#endif // MONITOR_CORE_MODELS_HPP