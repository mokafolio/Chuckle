#include <Chuckle/ChuckleLuaBindings.hpp>
#define PICLUASOL_IMPLEMENTATION
#include <PicLuaSol/PicLuaSol.hpp>

namespace pl = picLuaSol;

namespace chuckle
{
namespace detail
{

void _registerPicLuaBindings(sol::state_view _lua)
{
	pl::registerPic(_lua);
}

} // namespace detail
} // namespace chuckle
