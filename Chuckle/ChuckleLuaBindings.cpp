#include <Chuckle/ChuckleLuaBindings.hpp>

#include <DabLuaSol/DabLuaSol.hpp>
#include <LukeLuaSol/LukeLuaSol.hpp>
#include <PaperLuaSol/PaperLuaSol.hpp>

namespace sl = stickLuaSol;
namespace cl = crunchLuaSol;
namespace pl = paperLuaSol;
namespace ll = lukeLuaSol;
namespace dl = dabLuaSol;

using namespace stick;
using namespace paper;

namespace chuckle
{

// path utilities that need to be bound and dont have a better place to live in yet
namespace path
{
void longestCurves(Path * _path, Size _count, DynamicArray<Curve> & _output)
{
    _output.reserve(_path->curves().count());
    for (auto curve : _path->curves())
    {
        STICK_ASSERT(curve.path());
        _output.append(curve);
    }

    std::sort(_output.begin(), _output.end(), [](const Curve & _a, const Curve & _b) {
        return _a.length() > _b.length();
    });
    if (_output.count() > _count)
        _output.resize(_count);
}

void matchSegmentCount(Path * _a, Path * _b)
{
    if (_a->segmentCount() == _b->segmentCount())
        return;

    if (_a->segmentCount() > _b->segmentCount())
        std::swap(_a, _b);

    DynamicArray<Curve> tmp;
    while (_a->segmentCount() < _b->segmentCount())
    {
        Size diff = _b->segmentCount() - _a->segmentCount();
        longestCurves(_a, diff, tmp);

        for (Curve curve : tmp)
        {
            STICK_ASSERT(curve.path());
            curve.divideAtParameter(0.5);
        }
    }
}

void applyNoise(
    Item * _item, Float32 _noiseSeed, Float32 _noiseDiv, Float32 _noiseScale, Float32 _sampleDist)
{
    static crunch::PerlinNoise s_noise;
    if (_item->itemType() == ItemType::Path)
    {
        Path * p = static_cast<Path *>(_item);
        if (p->length() < _sampleDist)
            return;

        p->flattenRegular(_sampleDist, false);
        for (Segment seg : p->segments())
        {
            Vec2f pos = seg.position();
            Float32 n1 = s_noise.noise(pos.x / _noiseDiv, pos.y / _noiseDiv, _noiseSeed);
            Float32 ang = n1 * crunch::Constants<Float32>::twoPi();
            pos += Vec2f(std::cos(ang) * _noiseScale, std::sin(ang) * _noiseScale);
            seg.setPosition(pos);
        }
    }
    else
    {
        for (Item * child : _item->children())
            applyNoise(child, _noiseSeed, _noiseDiv, _noiseScale, _sampleDist);
    }
}

template <class T, class B>
T mix(const T & _a, const T & _b, B _fact)
{
    return _a + (_b - _a) * _fact;
}

void morph(Path * _a, Path * _b, Float32 _t, Path * _output)
{
    _output->removeSegments();
    for (Size i = 0; i < _a->segmentCount(); ++i)
    {
        Segment a = _a->segment(i);
        Segment b = _b->segment(i);
        _output->addSegment(mix(a.position(), b.position(), _t),
                            mix(a.handleIn(), b.handleIn(), _t),
                            mix(a.handleOut(), b.handleOut(), _t));
    }

    if (_a->isClosed())
        _output->closePath();
}

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

} // namespace path

void registerLuaBindings(sol::state_view _lua)
{
    ll::registerLuke(_lua);
    cl::registerCrunch(_lua);
    dl::registerDab(_lua);
    pl::registerPaper(_lua);

    sol::table chuckle = sl::ensureNamespaceTable(_lua, _lua.globals(), "__chuckle");
    chuckle.set_function("registerPathUtil", path::registerPathUtil);
}
} // namespace chuckle
