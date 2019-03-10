#ifndef CHUCKLE_CHUCKLELUABINDINGS_HPP
#define CHUCKLE_CHUCKLELUABINDINGS_HPP

#include <Stick/Platform.hpp>
#include <sol/sol.hpp>

namespace chuckle
{
    STICK_API void registerLuaBindings(sol::state_view _lua);
}

#endif //CHUCKLE_CHUCKLELUABINDINGS_HPP