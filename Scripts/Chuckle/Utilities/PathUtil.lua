local tbl = require("Chuckle.Utilities.TableUtil")
local mth = require("Chuckle.Utilities.MathUtil")
local util = {}

util.applyFBM = function(_path, _noiseSeed, _noiseDiv, _noiseScale, _sampleDist, _octaves)
    for i=1, _octaves, 1 do
        util.applyNoise(_path, _noiseSeed, _noiseDiv, _noiseScale, _sampleDist)
        _noiseDiv = _noiseDiv * 2.0
        _noiseScale = _noiseScale * 0.5
        _sampleDist = _sampleDist * 0.5
    end
end

util.arcify = function(_path, _sampleDist, _bClockwise)
    local ret = _path:document():createPath()
    local off = 0
    local len = _path:length()
    while off < len do
        local pos = _path:positionAt(math.min(len, off))
        if off == 0 then
            ret:addPoint(pos)
        else
            ret:arcTo(pos, _bClockwise)
        end
        off = off + _sampleDist
    end

    if off - len < _sampleDist then
        ret:arcTo(_path:positionAt(len), _bClockwise)
    end

    if _path:isClosed() then
        ret:closePath()
    end
    return ret
end

util.tubify = function(_path, _sampleDist, _radiusFn, _startFn, _endFn)
    local ret = _path:document():createPath()
    local len = _path:length()
    local off = 0
    local otherSidePos = {}
    local first, last, lastNormal, firstNormal

    local function addPoint(_off)
        local m = math.min(len, _off)
        local cl = _path:curveLocationAt(m)
        local n = cl:normal()
        local rad = _radiusFn(m, len, m / len)
        local p = cl:position() + n * rad
        ret:addPoint(p)
        table.insert(otherSidePos, cl:position() - n * rad)
        return cl, n, rad, p
    end

    while off < len do
        local cl, n, rad, p = addPoint(off)
        if off == 0 then
            first = p
            firstNormal = n
        end
        last = p
        lastNormal = n
        off = off + _sampleDist
    end

    if off - len < off then
        addPoint(len)
    end

    tbl.reverseInPlace(otherSidePos)
    _endFn(ret, last, otherSidePos[1], lastNormal)
    ret:arcTo(otherSidePos[1], true)

    table.remove(otherSidePos, 1)
    for _, v in ipairs(otherSidePos) do
        ret:addPoint(v)
        last = v
    end

    _startFn(ret, last, first, firstNormal * -1)

    return ret
end

return util
