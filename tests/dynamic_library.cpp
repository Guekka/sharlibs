#include <catch2/catch_all.hpp>
#include <sharlibs/dynamic_library.hpp>

TEST_CASE("DynamicLib::open")
{
    SECTION("Returns std::nullopt if the library does not exist")
    {
        auto lib = sharlibs::DynamicLib::open("does_not_exist");
        REQUIRE(!lib.has_value());
    }

    SECTION("Returns std::nullopt if the library cannot be opened")
    {
        auto lib = sharlibs::DynamicLib::open("tests/dynamic_library.cpp");
        REQUIRE(!lib.has_value());
    }

    SECTION("Returns a DynamicLib if the library can be opened")
    {
        auto lib = sharlibs::DynamicLib::open("libc.so.6");
        REQUIRE(lib.has_value());
    }
}

TEST_CASE("DynamicLib::call")
{
    SECTION("Returns std::nullopt if the symbol does not exist")
    {
        auto lib = sharlibs::DynamicLib::open("libc.so.6");
        REQUIRE(lib.has_value());

        auto result = lib->call<decltype(exit)>("does_not_exist", 0);
        REQUIRE(!result.has_value());
    }

    SECTION("Returns the result of the function if the symbol can be called")
    {
        auto lib = sharlibs::DynamicLib::open("libc.so.6");
        REQUIRE(lib.has_value());

        auto result = lib->call<decltype(tolower)>("tolower", 'A');
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 'a');
    }
}
