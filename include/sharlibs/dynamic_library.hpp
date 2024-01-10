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

class DynamicLib
{
    std::unique_ptr<void, detail::IntegralFunction<detail::native_close>> handle_;

    DynamicLib(native_handle handle)
        : handle_{handle}
    {
    }

    [[nodiscard]] auto get_symbol(const char *name) -> void * { return dlsym(handle_.get(), name); }

public:
    [[nodiscard]] static auto open(const char *path) -> std::optional<DynamicLib>
    {
        if (auto handle = detail::native_open(path); handle != nullptr)
            return DynamicLib{handle};

        return std::nullopt;
    }

    template<class Func, class... Args>
    [[nodiscard]] auto call(const char *name, Args &&...args)
        -> std::optional<std::invoke_result_t<Func, Args...>>
        requires std::is_invocable_v<Func, Args...> && (!std::is_void_v<std::invoke_result_t<Func, Args...>>)
    {
        if (auto symbol = get_symbol(name); symbol != nullptr)
        {
            auto function = reinterpret_cast<std::add_pointer_t<Func>>(symbol);
            return std::invoke(function, std::forward<Args>(args)...);
        }

        return std::nullopt;
    }

    template<class Func, class... Args>
    [[nodiscard]] auto call(const char *name, Args &&...args) -> std::optional<std::monostate>
        requires std::is_invocable_v<Func, Args...> && std::is_void_v<std::invoke_result_t<Func, Args...>>
    {
        if (auto symbol = get_symbol(name); symbol != nullptr)
        {
            auto function = reinterpret_cast<std::add_pointer_t<Func>>(symbol);
            std::invoke(function, std::forward<Args>(args)...);
            return std::monostate{};
        }

        return std::nullopt;
    }
};

static_assert(std::is_nothrow_move_constructible_v<DynamicLib>);
static_assert(std::is_nothrow_move_assignable_v<DynamicLib>);
static_assert(std::is_nothrow_destructible_v<DynamicLib>);
static_assert(!std::is_nothrow_default_constructible_v<DynamicLib>);

} // namespace sharlibs
