#include <sharlibs/dynamic_library.hpp>

#include <iostream>

using ExitType = void(int);

auto main() -> int
{
    auto lib = sharlibs::DynamicLib::open("libc.so.6");
    if (!lib.has_value())
    {
        std::cerr << "Failed to open dynamic library\n";
        return 1;
    }

    auto success = lib->call<ExitType>("exit", 0);
    if (!success.has_value())
    {
        std::cerr << "Failed to call exit\n";
        return 1;
    }

    // This should not be reached
    std::cerr << "Exit symbol found but exit was not called\n";
    return 2;
}
