local util = {}

util.randomSign = function()
    if random(0, 1) >= 0.5 then return 1 else return -1 end
end

util.angle = function(_a, _b)
    return math.atan2(_a.y, _a.x) - math.atan2(_b.y, _b.x)
end

util.round = function(_val)
    return _val >= 0 and math.floor(_val + 0.5) or math.ceil(_val - 0.5)
end

util.mix = function(_a, _b, _fact)
    return _a + (_b - _a) * _fact
end

util.sortClockwise = function(_points)
    local center = Vec2(0)
    for k,v in ipairs(_points) do
        center = center + v
    end
    center = center / #_points

    table.sort(_points, function(_a, _b)
        if _a.x - center.x >= 0 and _b.x - center.x < 0 then
            return true
        end
        if _a.x - center.x < 0 and _b.x - center.x >= 0 then
            return false
        end
        if _a.x - center.x == 0 and _b.x - center.x == 0 then
            if _a.y - center.y >= 0 or _b.y - center.y >= 0 then
                return a.y > b.y
            end
            return b.y > a.y
        end
        local det = (_a.x - center.x) * (_b.y - center.y) - (_b.x - center.x) * (_a.y - center.y)
        if det < 0 then return true end
        if det > 0 then return false end

        local d1 = (_a.x - center.x) * (_a.x - center.x) + (_a.y - center.y) * (_a.y - center.y);
        local d2 = (_b.x - center.x) * (_b.x - center.x) + (_b.y - center.y) * (_b.y - center.y);
        return d1 > d2
    end)
end

util.shortestAngle = function(_ang)
    local twoPi = 2.0 * math.pi
    _ang = _ang % twoPi
    if _ang < -math.pi then
        _ang = _ang + twoPi
    elseif _ang > math.pi then
        _ang = _ang - twoPi
    end
    return _ang
end

return util
