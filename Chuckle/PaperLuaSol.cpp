#include <Chuckle/ChuckleLuaBindings.hpp>
#define PAPERLUASOL_IMPLEMENTATION
#include <PaperLuaSol/PaperLuaSol.hpp>

namespace pl = paperLuaSol;

namespace chuckle
{
namespace detail
{

void _registerPaperLuaBindings(sol::state_view _lua)
{
	pl::registerPaper(_lua);
}

} // namespace detail
} // namespace chuckle
