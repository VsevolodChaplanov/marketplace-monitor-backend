#ifndef MONITOR_UTILITY_STRONG_TYPE_HPP
#define MONITOR_UTILITY_STRONG_TYPE_HPP

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace mo::utility {
    template<typename T, typename Tag>
        requires(std::is_object_v<T>)
    struct strong_type final {

        template<typename U>
        explicit strong_type(U&& value)
            requires(!std::same_as<U, strong_type<T, Tag>> && std::constructible_from<T, U>)
            : value_(std::forward<U>(value)) {}

        template<typename... Ts>
        explicit strong_type(Ts&&... args)
            requires(std::constructible_from<T, Ts && ...>)
            : value_(std::forward<Ts>(args)...) {}

        strong_type(const strong_type&) = default;
        strong_type(strong_type&&) noexcept = default;
        strong_type& operator=(const strong_type&) = default;
        strong_type& operator=(strong_type&&) noexcept = default;

        auto operator*() const noexcept -> const T& { return value_; }
        auto operator*() noexcept -> T& { return value_; }

        auto operator->() const noexcept -> const T* { return std::addressof(value_); }
        auto operator->() noexcept -> T* { return std::addressof(value_); }

        friend constexpr auto operator<=>(const strong_type&, const strong_type&) noexcept = default;

        ~strong_type() = default;

    private:
        T value_;
    };
} // namespace mo::utility

#endif // MONITOR_UTILITY_STRONG_TYPE_HPP