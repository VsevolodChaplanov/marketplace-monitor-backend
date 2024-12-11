#include <catch2/catch_test_macros.hpp>
#include <monitor/core/repository.hpp>

namespace mo::core::testing {
    TEST_CASE("basic object pool unit tests") {
        using object = int;
        using pool = basic_object_pool<object>;

        constexpr std::size_t amount{10};

        auto instance = pool::make<amount>([] { return 0; });

        auto int_object_wrapped = instance->get();

        CHECK(*int_object_wrapped == 0);
    }
} // namespace mo::core::testing