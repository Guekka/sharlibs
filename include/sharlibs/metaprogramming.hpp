#include <type_traits>

namespace sharlibs::detail {
template<auto Function>
using IntegralFunction = std::integral_constant<decltype(Function), Function>;
} // namespace sharlibs::detail
