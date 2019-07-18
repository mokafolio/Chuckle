#include <Chuckle/ChuckleLuaBindings.hpp>
#define LUKELUASOL_IMPLEMENTATION
#include <LukeLuaSol/LukeLuaSol.hpp>

namespace ll = lukeLuaSol;

namespace chuckle
{
namespace detail
{

void _registerLukeLuaBindings(sol::state_view _lua)
{
	ll::registerLuke(_lua);
}

} // namespace detail
} // namespace chuckle
