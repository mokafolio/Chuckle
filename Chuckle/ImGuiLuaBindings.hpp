#ifndef CHUCKLE_IMGUILUABINDINGS_HPP
#define CHUCKLE_IMGUILUABINDINGS_HPP

#include <Stick/Platform.hpp>
#include <type_traits>
#include <sol/sol.hpp>

namespace chuckle
{
    STICK_API void registerImGuiBindings(sol::state_view _lua);
}

#endif //CHUCKLE_IMGUILUABINDINGS_HPP
