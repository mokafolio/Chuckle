#include <Chuckle/ChuckleLuaBindings.hpp>

#include <ChuckleCore/ChuckleCore.hpp>
#include <DabLuaSol/DabLuaSol.hpp>
#include <LukeLuaSol/LukeLuaSol.hpp>
#include <PaperLuaSol/PaperLuaSol.hpp>

namespace sl = stickLuaSol;
namespace cl = crunchLuaSol;
namespace pl = paperLuaSol;
namespace ll = lukeLuaSol;
namespace dl = dabLuaSol;

namespace chuckle
{
// called from lua when utils are imported
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
    printf("REGISTERING BIIITHC\n");
    // first register all existing lua bindings
    ll::registerLuke(_lua);
    cl::registerCrunch(_lua);
    dl::registerDab(_lua);
    pl::registerPaper(_lua);

    // chuckle specific bindings

    sol::table globals = _lua.globals();
    // this is a hidden table to hold internal functions
    sol::table chuckle = sl::ensureNamespaceTable(_lua, globals, "__chuckle");
    chuckle.set_function("registerPathUtil", registerPathUtil);

    globals.new_usertype<RenderWindow>(
        "RenderWindow",
        sol::base_classes,
        sol::bases<luke::Window>(),
        sol::call_constructor,
        sol::factories([](const WindowSettings & _settings, sol::this_state _s) {
            sol::state_view L(_s);
            std::unique_ptr<RenderWindow> wnd(new RenderWindow);
            auto err = wnd->open(_settings);
            if (err)
                return sol::make_object(L, err);
            return sol::make_object(L, std::move(wnd));
        }),
        "enableDefaultUI",
        sol::overload(&RenderWindow::enableDefaultUI,
                      [](RenderWindow * _self) {
                          return _self->enableDefaultUI(
                              stick::path::join(executableDirectoryName(),
                                                "../Assets/RobotoMono-Regular.ttf").cString(),
                              14);
                      }),
        "frameImage",
        sol::overload((ImageUniquePtr(RenderWindow::*)()) & RenderWindow::frameImage,
                      (ImageUniquePtr(RenderWindow::*)(UInt32, UInt32, UInt32, UInt32)) &
                          RenderWindow::frameImage),
        "saveFrame",
        sol::overload((Error(RenderWindow::*)(const char *)) & RenderWindow::saveFrame,
                      (Error(RenderWindow::*)(const char *, UInt32, UInt32, UInt32, UInt32)) &
                          RenderWindow::saveFrame),
        "renderDevice",
        &RenderWindow::renderDevice,
        "setDrawFunction",
        [](RenderWindow * _self, sol::function _fn) {
            _self->setDrawFunction([_fn](Float64 _duration) { return _fn(_duration); });
        },
        "run",
        &RenderWindow::run,
        "fps",
        &RenderWindow::fps);

    globals.new_usertype<PaperWindow>(
        "PaperWindow",
        sol::base_classes,
        sol::bases<RenderWindow, luke::Window>(),
        sol::call_constructor,
        sol::factories([](const WindowSettings & _settings, sol::this_state _s) {
            sol::state_view L(_s);
            std::unique_ptr<PaperWindow> wnd(new PaperWindow);
            auto err = wnd->open(_settings);
            if (err)
                return sol::make_object(L, err);
            return sol::make_object(L, std::move(wnd));
        }),
        "document",
        &PaperWindow::document,
        "paperRenderer",
        &PaperWindow::paperRenderer,
        "drawDocument",
        &PaperWindow::drawDocument,
        "setAutoResize",
        &PaperWindow::setAutoResize,
        "autoResize",
        &PaperWindow::autoResize);
}
} // namespace chuckle
