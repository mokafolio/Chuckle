#include <Chuckle/ChuckleLuaBindings.hpp>
#define DABLUASOL_IMPLEMENTATION
#include <DabLuaSol/DabLuaSol.hpp>

namespace dl = dabLuaSol;

namespace chuckle
{
namespace detail
{

void _registerDabLuaBindings(sol::state_view _lua)
{
	dl::registerDab(_lua);
}

} // namespace detail
} // namespace chuckle