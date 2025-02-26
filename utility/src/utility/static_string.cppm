module;

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <string>
#include <string_view>

export module utility.static_string;

export namespace utility {
    /**
     * @brief class for compile time struct tagging
     */
    template<std::size_t N> struct static_string final {
        constexpr static_string() noexcept = default;

        constexpr static_string(const static_string&) noexcept = default;
        constexpr static_string(static_string&&) noexcept = default;
        constexpr auto operator=(const static_string&) noexcept -> static_string& = default;
        constexpr auto operator=(static_string&&) noexcept -> static_string& = default;

        // NOLINTNEXTLINE (cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
        constexpr explicit(false) static_string(const char (&data)[N + 1]) noexcept {
            std::ranges::copy(data, m_data.begin());
        }

        template<char... Data> constexpr explicit static_string() noexcept : m_data({Data...}) {}

        template<std::input_iterator TIt, std::sentinel_for<TIt> TS>
        constexpr explicit static_string(TIt begin, TS end) : m_data(begin, end) {}

        [[nodiscard]] constexpr auto empty() const noexcept -> bool { return N == 0; }

        constexpr auto data() const noexcept { return m_data.data(); }

        constexpr auto data() noexcept { return m_data.data(); }

        constexpr auto size() const noexcept { return N; }

        constexpr auto c_str() const noexcept { return data(); }

        constexpr auto begin() const noexcept { return m_data.begin(); }

        constexpr auto end() const noexcept { return m_data.end(); }

        constexpr auto begin() noexcept { return m_data.begin(); }

        constexpr auto end() noexcept { return m_data.end(); }

        [[nodiscard]] constexpr auto view() const noexcept -> std::string_view { return {data(), N}; }

        [[nodiscard]] constexpr auto string() const -> std::string { return std::string(begin(), begin() + N); }

        constexpr explicit operator std::string_view() const noexcept { return view(); }

        constexpr explicit operator std::string() const noexcept { return string(); }

        constexpr auto operator[](std::size_t index) const noexcept { return m_data[index]; }

        constexpr ~static_string() noexcept = default;

        std::array<char, N + 1> m_data{}; // NOLINT misc-non-private-member-variables-in-classes
    };

    namespace literals {
        // NOLINTNEXTLINE misc-use-internal-linkage
        template<static_string VString> consteval auto operator""_fs() noexcept { return VString; }
    } // namespace literals

    // NOLINTNEXTLINE misc-use-internal-linkage
    template<std::size_t N> constexpr auto format_as(const static_string<N>& string) -> std::string_view {
        return string.view();
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator<=>(const static_string<K>& self, const static_string<M>& other) noexcept {
        return std::lexicographical_compare_three_way(self.begin(), self.end(), other.begin(), other.end());
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator<=>(const static_string<K>& lhs, const char (&rhs)[M]) noexcept {
        return lhs <=> static_string{rhs};
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator<=>(const char (&lhs)[K], const static_string<M>& rhs) noexcept {
        return static_string{lhs} <=> static_string{rhs};
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator==(const static_string<K>& self, const static_string<M>& other) noexcept {
        if (K != M) {
            return false;
        }

        return std::ranges::equal(self, other);
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator==(const static_string<K>& lhs, const char (&rhs)[M]) noexcept {
        return lhs == static_string{rhs};
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator==(const char (&lhs)[K], const static_string<M>& rhs) noexcept {
        return rhs == lhs;
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator!=(const static_string<K>& self, const static_string<M>& other) noexcept {
        return !(self == other);
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator!=(const static_string<K>& lhs, const char (&rhs)[M]) noexcept {
        return !(lhs == rhs);
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator!=(const char (&lhs)[K], const static_string<M>& rhs) noexcept {
        return !(rhs == lhs);
    }

    template<std::size_t K, std::size_t M>
    constexpr auto operator+(const static_string<K>& lhs, const static_string<M>& rhs) -> static_string<K + M> {
        static_string<K + M> result;
        std::ranges::copy(lhs, result.begin());
        std::ranges::copy(rhs, result.begin() + K);
        return result;
    }

    template<size_t K, size_t M>
    constexpr auto operator+(const char (&lhs)[K], const static_string<M>& rhs) -> static_string<K - 1 + M> {
        static_string lhs2{lhs};
        return lhs2 + rhs;
    }

    template<size_t K, size_t M>
    constexpr auto operator+(const static_string<K>& lhs, const char (&rhs)[M]) -> static_string<K + M - 1> {
        static_string rhs2{rhs};
        return lhs + rhs2;
    }

    /** CTAD helpers for static_string */

    template<std::size_t N> static_string(const char (&)[N]) -> static_string<N - 1>;

    template<std::size_t N> static_string(const char*, std::integral_constant<std::size_t, N>) -> static_string<N - 1>;

    template<char... VData> static_string() -> static_string<sizeof...(VData)>;
} // namespace utility