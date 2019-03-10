#include <Chuckle/ChuckleLuaBindings.hpp>

#include <DabLuaSol/DabLuaSol.hpp>
#include <LukeLuaSol/LukeLuaSol.hpp>
#include <PaperLuaSol/PaperLuaSol.hpp>
#include <ChuckleCore/ChuckleCore.hpp>

//@NOTE: for glReadPixels...this is only temporary i hope :)
#include <Dab/Libs/GL/gl3w.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace sl = stickLuaSol;
namespace cl = crunchLuaSol;
namespace pl = paperLuaSol;
namespace ll = lukeLuaSol;
namespace dl = dabLuaSol;

namespace chuckle
{
//called from lua when utils are imported
void registerPathUtil(sol::table _table)
{
    _table.set_function("longestCurves", [](Path * _path, Size _count, sol::this_state _s) {
        sol::state_view L(_s);
        sol::table tbl = L.create_table();
        stick::DynamicArray<Curve> output;
        path::longestCurves(_path, _count, output);
        for (Size i = 0; i < output.count(); ++i)
            tbl[i + 1] = output[i];
        return tbl;
    });
    _table.set_function("matchSegmentCount", path::matchSegmentCount);
    _table.set_function("applyNoise", path::applyNoise);
    _table.set_function("morph", path::morph);
}

void registerLuaBindings(sol::state_view _lua)
{
    ll::registerLuke(_lua);
    cl::registerCrunch(_lua);
    dl::registerDab(_lua);
    pl::registerPaper(_lua);

    sol::table chuckle = sl::ensureNamespaceTable(_lua, _lua.globals(), "__chuckle");
    chuckle.set_function("registerPathUtil", registerPathUtil);
}
} // namespace chuckle
