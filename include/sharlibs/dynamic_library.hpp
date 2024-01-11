#pragma once

#include <dlfcn.h>
#include <sharlibs/metaprogramming.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <variant>

namespace sharlibs {

using native_handle = void *;

namespace detail {
[[nodiscard]] inline auto native_open(const char *path) -> native_handle
{
    return dlopen(path, RTLD_LAZY);
}

inline void native_close(native_handle handle)
{
    dlclose(handle);
}
} // namespace detail

template<detail::FixedString function_name, class Prototype>
struct DynamicFunction
{
    constexpr DynamicFunction() noexcept = default;

    using function_type                       = Prototype;
    constexpr static detail::FixedString name = function_name;
};

template<detail::FixedString library>
class DynamicLib
{
    std::unique_ptr<void, detail::IntegralFunction<detail::native_close>> handle_;

    DynamicLib(native_handle handle)
        : handle_{handle}
    {
    }

    [[nodiscard]] auto get_symbol(const char *name) -> void * { return dlsym(handle_.get(), name); }

public:
    [[nodiscard]] static auto open() -> std::optional<DynamicLib>
    {
        if (auto handle = detail::native_open(library.chars); handle != nullptr)
            return DynamicLib{handle};

        return std::nullopt;
    }

    template<DynamicFunction func, class... Args, class Func = typename decltype(func)::function_type>
    [[nodiscard]] auto call(Args &&...args) -> std::optional<std::invoke_result_t<Func, Args...>>
        requires std::is_invocable_v<Func, Args...> && (!std::is_void_v<std::invoke_result_t<Func, Args...>>)
    {
        if (auto symbol = get_symbol(func.name.chars); symbol != nullptr)
        {
            auto function = reinterpret_cast<std::add_pointer_t<Func>>(symbol);
            return std::invoke(function, std::forward<Args>(args)...);
        }

        return std::nullopt;
    }

    template<DynamicFunction func, class... Args, class Func = typename decltype(func)::function_type>
    [[nodiscard]] auto call(Args &&...args) -> std::optional<std::monostate>
        requires std::is_invocable_v<Func, Args...> && std::is_void_v<std::invoke_result_t<Func, Args...>>
    {
        if (auto symbol = get_symbol(func.name.chars); symbol != nullptr)
        {
            auto function = reinterpret_cast<std::add_pointer_t<Func>>(symbol);
            std::invoke(function, std::forward<Args>(args)...);
            return std::monostate{};
        }

        return std::nullopt;
    }
};

namespace detail {

using ExampleLib = DynamicLib<FixedString{"libc.so.6"}>;

static_assert(std::is_nothrow_move_constructible_v<ExampleLib>);
static_assert(std::is_nothrow_move_assignable_v<ExampleLib>);
static_assert(std::is_nothrow_destructible_v<ExampleLib>);
static_assert(!std::is_nothrow_default_constructible_v<ExampleLib>);
} // namespace detail
} // namespace sharlibs
