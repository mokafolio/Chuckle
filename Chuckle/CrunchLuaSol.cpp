#include <Chuckle/ChuckleLuaBindings.hpp>
#define CRUNCHLUASOL_IMPLEMENTATION
#include <CrunchLuaSol/CrunchLuaSol.hpp>

namespace cl = crunchLuaSol;

namespace chuckle
{
namespace detail
{

void _registerCrunchLuaBindings(sol::state_view _lua)
{
	cl::registerCrunch(_lua);
}

} // namespace detail
} // namespace chuckle