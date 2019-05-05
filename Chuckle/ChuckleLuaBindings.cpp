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

namespace detail
{
void _tableToVec2fData(sol::table _table, DynamicArray<Vec2f> & _outData)
{
    _outData.reserve(_table.size());
    for (auto & kv : _table)
    {
        if (kv.second.is<Vec2f>())
        {
            _outData.append(kv.second.as<Vec2f>());
        }
        else if (kv.second.is<sol::table>())
        {
            sol::table tbl = kv.second.as<sol::table>();
            STICK_ASSERT(tbl.size() == 2);
            _outData.append(Vec2f(tbl[1], tbl[2]));
        }
    }
}

template <class ItemT>
void _tableToPaperItems(sol::table _table, DynamicArray<ItemT *> & _outData)
{
    _outData.reserve(_table.size());
    for (auto & kv : _table)
    {
        STICK_ASSERT(kv.second.is<ItemT *>());
        _outData.append(kv.second.as<ItemT *>());
    }
}
} // namespace detail

void registerLuaBindings(sol::state_view _lua)
{
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

    globals.new_usertype<ImGuiInterface>(
        "ImGuiInterface",
        sol::base_classes,
        sol::bases<stick::EventForwarder>(),
        "new",
        sol::no_constructor, // can't be directly constructed for now
        "newFrame",
        &ImGuiInterface::newFrame,
        "finalizeFrame",
        &ImGuiInterface::finalizeFrame);

    globals.new_usertype<QuickDraw>(
        "QuickDraw",
        "new",
        sol::no_constructor, // can't be directly constructed for now
        "setViewport",
        &QuickDraw::setViewport,
        "setTransform",
        sol::overload((void (QuickDraw::*)(const Mat32f &)) & QuickDraw::setTransform,
                      (void (QuickDraw::*)(const Mat4f &)) & QuickDraw::setTransform),
        "setProjection",
        sol::overload((void (QuickDraw::*)(const Mat32f &)) & QuickDraw::setProjection,
                      (void (QuickDraw::*)(const Mat4f &)) & QuickDraw::setProjection),
        "pushTransform",
        &QuickDraw::pushTransform,
        "popTransform",
        &QuickDraw::popTransform,
        "pushProjection",
        &QuickDraw::pushProjection,
        "popProjection",
        &QuickDraw::popProjection,
        "applyTransform",
        sol::overload((void (QuickDraw::*)(const Mat32f &)) & QuickDraw::applyTransform,
                      (void (QuickDraw::*)(const Mat4f &)) & QuickDraw::applyTransform),
        "setColor",
        &QuickDraw::setColor,
        "transform",
        &QuickDraw::transform,
        "projection",
        &QuickDraw::projection,
        "transformProjection",
        &QuickDraw::transformProjection,
        "addToPass",
        &QuickDraw::addToPass,
        "flush",
        &QuickDraw::flush,
        "rect",
        &QuickDraw::rect,
        "lineRect",
        &QuickDraw::lineRect,
        "lines",
        [](QuickDraw * _self, sol::table _table) {
            stick::DynamicArray<Vec2f> data;
            detail::_tableToVec2fData(_table, data);
            _self->lines(&data[0], data.count());
        },
        "lineStrip",
        sol::overload(
            [](QuickDraw * _self, sol::table _table) {
                stick::DynamicArray<Vec2f> data;
                detail::_tableToVec2fData(_table, data);
                _self->lineStrip(&data[0], data.count());
            },
            [](QuickDraw * _self, sol::table _table, bool _bClose) {
                stick::DynamicArray<Vec2f> data;
                detail::_tableToVec2fData(_table, data);
                _self->lineStrip(&data[0], data.count(), _bClose);
            }),
        "points",
        [](QuickDraw * _self, sol::table _table) {
            stick::DynamicArray<Vec2f> data;
            detail::_tableToVec2fData(_table, data);
            _self->points(&data[0], data.count());
        },
        "rects",
        [](QuickDraw * _self, sol::table _table, Float32 _radius) {
            stick::DynamicArray<Vec2f> data;
            detail::_tableToVec2fData(_table, data);
            _self->rects(&data[0], data.count(), _radius);
        },
        "lineRects",
        [](QuickDraw * _self, sol::table _table, Float32 _radius) {
            stick::DynamicArray<Vec2f> data;
            detail::_tableToVec2fData(_table, data);
            _self->lineRects(&data[0], data.count(), _radius);
        });

    globals.new_usertype<RenderWindow>(
        "RenderWindow",
        sol::base_classes,
        sol::bases<luke::Window, stick::EventForwarder>(),
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
                                                "../Assets/RobotoMono-Regular.ttf")
                                  .cString(),
                              14);
                      }),
        "setShowWindowMetrics",
        &RenderWindow::setShowWindowMetrics,
        "toggleShowWindowMetrics",
        &RenderWindow::toggleShowWindowMetrics,
        "isShowingWindowMetrics",
        &RenderWindow::isShowingWindowMetrics,
        "imGuiInterface",
        &RenderWindow::imGuiInterface,
        "quickDraw",
        &RenderWindow::quickDraw,
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
        &RenderWindow::fps,
        "frameCount",
        &RenderWindow::frameCount,
        "drawPathOutline",
        sol::overload(
            &RenderWindow::drawPathOutline,
            [](RenderWindow * _self, Path * _path, RenderInterface & _ri, const ColorRGBA & _col) {
                _self->drawPathOutline(_path, _ri, _col);
            }),
        "drawMultiplePathOutlines",
        [](RenderWindow * _self,
           sol::table _paths,
           RenderInterface & _ri,
           const ColorRGBA & _col,
           bool _bDrawChildren) {
            DynamicArray<Path *> items;
            detail::_tableToPaperItems<Path>(_paths, items);
            _self->drawMultiplePathOutlines(&items[0], items.count(), _ri, _col, _bDrawChildren);
        },
        "drawPathHandles",
        sol::overload(&RenderWindow::drawPathHandles,
                      [](RenderWindow * _self, Path * _path, const ColorRGBA & _col) {
                          _self->drawPathHandles(_path, _col);
                      },
                      [](RenderWindow * _self, Path * _path, const ColorRGBA & _col, Float32 _rad) {
                          _self->drawPathHandles(_path, _col, _rad);
                      }),
        "drawMultiplePathHandles",
        [](RenderWindow * _self,
           sol::table _paths,
           const ColorRGBA & _col,
           Float32 _rad,
           bool _bDrawChildren) {
            DynamicArray<Path *> items;
            detail::_tableToPaperItems<Path>(_paths, items);
            _self->drawMultiplePathHandles(&items[0], items.count(), _col, _rad, _bDrawChildren);
        },
        "drawItemBoundingBox",
        sol::overload(&RenderWindow::drawItemBoundingBox,
                      [](RenderWindow * _self, Item * _item, const ColorRGBA & _col) {
                          _self->drawItemBoundingBox(_item, _col);
                      }),
        "drawMultipleItemBoundingBoxes",
        [](RenderWindow * _self, sol::table _paths, const ColorRGBA & _col, bool _bDrawChildren) {
            DynamicArray<Item *> items;
            detail::_tableToPaperItems<Item>(_paths, items);
            _self->drawMultipleItemBoundingBoxes(&items[0], items.count(), _col, _bDrawChildren);
        });

    globals.new_usertype<PaperWindow>(
        "PaperWindow",
        sol::base_classes,
        sol::bases<RenderWindow, luke::Window, stick::EventForwarder>(),
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
        &PaperWindow::autoResize,
        "drawPathOutline",
        sol::overload(&PaperWindow::drawPathOutline,
                      [](PaperWindow * _self, Path * _path, const ColorRGBA & _col) {
                          _self->drawPathOutline(_path, _col);
                      }),
        "drawMultiplePathOutlines",
        [](PaperWindow * _self, sol::table _paths, const ColorRGBA & _col, bool _bDrawChildren) {
            DynamicArray<Path *> items;
            detail::_tableToPaperItems<Path>(_paths, items);
            _self->drawMultiplePathOutlines(&items[0], items.count(), _col, _bDrawChildren);
        });

    globals.new_enum("FileDialogFlags",
                     "Open",
                     FileDialogFlags::Open,
                     "Save",
                     FileDialogFlags::Save,
                     "Directory",
                     FileDialogFlags::Directory,
                     "OverwriteConfirmation",
                     FileDialogFlags::OverwriteConfirmation);

    globals.set_function("fileDialog",
                         [](FileDialogFlags _flags,
                            sol::optional<const char *> _filter,
                            sol::optional<const char *> _path,
                            sol::optional<const char *> _fileName) {
                             return fileDialog(_flags,
                                               _filter ? *_filter : NULL,
                                               _path ? *_path : NULL,
                                               _fileName ? *_fileName : NULL);
                         });
}
} // namespace chuckle
