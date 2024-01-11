#include <type_traits>

namespace sharlibs::detail {

template<auto Function>
using IntegralFunction = std::integral_constant<decltype(Function), Function>;

template<size_t Length>
struct FixedString
{
    constexpr FixedString(const char (&arr)[Length + 1]) noexcept
    {
        for (size_t i = 0; i < Length; ++i)
            chars[i] = arr[i];
    }

    char chars[Length + 1] = {}; // +1 for null terminator

    [[nodiscard]] constexpr auto size() const noexcept -> size_t { return Length; }

    [[nodiscard]] constexpr auto operator[](size_t index) const noexcept -> char { return chars[index]; }
};

template<size_t N>
FixedString(const char (&arr)[N]) -> FixedString<N - 1>; // Drop the null terminator

template<class What, class... Args>
struct is_present : std::disjunction<std::is_same<What, Args>...>
{
};

template<class What, class... Args>
constexpr auto is_present_v = is_present<What, Args...>::value;

} // namespace sharlibs::detail
