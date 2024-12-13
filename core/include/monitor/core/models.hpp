#ifndef MONITOR_CORE_MODELS_HPP
#define MONITOR_CORE_MODELS_HPP

#include "boost/mysql/pfr.hpp"
#include "boost/mysql/static_results.hpp"
#include <cstdint>

namespace mo::core {

    using id_t = std::uint64_t;

    struct base_sku_model {
        std::uint64_t sku{0};
        std::string url;
        double price;

        constexpr friend auto operator<=>(const base_sku_model&, const base_sku_model&) noexcept = default;
    };

    struct id_field {
        id_t id{0};
        constexpr friend auto operator<=>(const id_field&, const id_field&) noexcept = default;
    };

    struct sku_model : id_field, base_sku_model {
        constexpr friend auto operator<=>(const sku_model&, const sku_model&) noexcept = default;
    };

    using sku_model_sql_t = boost::mysql::pfr_by_name<sku_model>;
    using sku_model_sql_result_t = boost::mysql::static_results<sku_model_sql_t>;
} // namespace mo::core

#endif // MONITOR_CORE_MODELS_HPP