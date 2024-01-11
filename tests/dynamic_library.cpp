#include <catch2/catch_all.hpp>
#include <sharlibs/dynamic_library.hpp>

TEST_CASE("DynamicLib::open")
{
    SECTION("Returns std::nullopt if the library does not exist")
    {
        auto lib = sharlibs::DynamicLib<"does_not_exist">::open();
        REQUIRE(!lib.has_value());
    }

    SECTION("Returns std::nullopt if the library cannot be opened")
    {
        auto lib = sharlibs::DynamicLib<"tests/dynamic_library.cpp">::open();
        REQUIRE(!lib.has_value());
    }

    SECTION("Returns a DynamicLib if the library can be opened")
    {
        auto lib = sharlibs::DynamicLib<"libc.so.6">::open();
        REQUIRE(lib.has_value());
    }
}

TEST_CASE("DynamicLib::call")
{
    SECTION("Returns std::nullopt if the symbol does not exist")
    {
        auto lib = sharlibs::DynamicLib<"libc.so.6">::open();
        REQUIRE(lib.has_value());

        constexpr auto f_exit = sharlibs::DynamicFunction<"does_not_exist", decltype(exit)>{};
        auto result           = lib->call<f_exit>(0);
        REQUIRE(!result.has_value());
    }

    SECTION("Returns the result of the function if the symbol can be called")
    {
        constexpr auto f_tolower = sharlibs::DynamicFunction<"tolower", decltype(tolower)>{};

        auto lib = sharlibs::DynamicLib<"libc.so.6", f_tolower>::open();
        REQUIRE(lib.has_value());

        auto result = lib->call<f_tolower>('A');
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 'a');
    }
}
