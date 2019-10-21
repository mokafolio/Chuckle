#ifndef CHUCKLE_CHUCKLELUABINDINGS_HPP
#define CHUCKLE_CHUCKLELUABINDINGS_HPP

#include <Stick/Platform.hpp>
#include <type_traits>
#include <sol/sol.hpp>

namespace chuckle
{

// helpers to put all the bindings into their own .cpp/translation unit to slightly counter balance
// the crazy sol related compile times for the lua bindings.
namespace detail
{
STICK_API void _registerCrunchLuaBindings(sol::state_view _lua);
STICK_API void _registerDabLuaBindings(sol::state_view _lua);
STICK_API void _registerLukeLuaBindings(sol::state_view _lua);
STICK_API void _registerPaperLuaBindings(sol::state_view _lua);
STICK_API void _registerPicLuaBindings(sol::state_view _lua);
} // namespace detail

STICK_API void registerLuaBindings(sol::state_view _lua);
} // namespace chuckle

#endif // CHUCKLE_CHUCKLELUABINDINGS_HPP
