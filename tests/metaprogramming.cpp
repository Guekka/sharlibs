#include <catch2/catch_all.hpp>
#include <sharlibs/metaprogramming.hpp>

TEST_CASE("IntegralFunction")
{
    SECTION("Can be constructed from a function pointer")
    {
        using F = sharlibs::detail::IntegralFunction<exit>;
        STATIC_REQUIRE(std::is_same_v<F, sharlibs::detail::IntegralFunction<exit>>);
        STATIC_REQUIRE(F::value == exit);
    }
}

template<sharlibs::detail::FixedString String>
struct S
{
};

TEST_CASE("FixedString")
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    constexpr const char c_str[] = "Hello, world!";

    SECTION("Can be constructed from a string literal")
    {
        constexpr sharlibs::detail::FixedString string{c_str};
        STATIC_REQUIRE(string.size() == 13);
        STATIC_REQUIRE(string[0] == 'H');
        STATIC_REQUIRE(string[10] == 'l');
        STATIC_REQUIRE(string[12] == '!');
        STATIC_REQUIRE(string[13] == '\0');
    }

    SECTION("Is implicitly constructible")
    {
        constexpr S<c_str> s;
        STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(s)>, S<c_str>>);
    }
}
