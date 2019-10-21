#ifndef CHUCKLE_BOX2DLUABINDINGS_HPP
#define CHUCKLE_BOX2DLUABINDINGS_HPP

#include <Stick/Platform.hpp>
#include <type_traits>
#include <sol/sol.hpp>

namespace chuckle
{
    STICK_API void registerBox2DBindings(sol::state_view _lua);
}

#endif //CHUCKLE_BOX2DLUABINDINGS_HPP
