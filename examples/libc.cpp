#include <sharlibs/dynamic_library.hpp>

#include <iostream>

constexpr auto f_exit = sharlibs::DynamicFunction<"exit", int(int)>{};

auto main() -> int
{
    auto lib = sharlibs::DynamicLib<"libc.so.6", f_exit>::open();
    if (!lib.has_value())
    {
        std::cerr << "Failed to open dynamic library\n";
        return 1;
    }

    auto success = lib->call<f_exit>(0);
    if (!success.has_value())
    {
        std::cerr << "Failed to call exit\n";
        return 1;
    }

    // This should not be reached
    std::cerr << "Exit symbol found but exit was not called\n";
    return 2;
}
