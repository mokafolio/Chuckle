local util = {}

util.randomEntry = function(_presetTable)
    return _presetTable[math.floor(random(1, #_presetTable + 1))]
end

util.randomEntryAndRemove = function(_presetTable)
    local idx = math.floor(random(1, #_presetTable + 1))
    return table.remove(_presetTable, idx)
end

util.randomEntryWithIndex = function(_presetTable)
    local idx = math.floor(random(1, #_presetTable + 1))
    return _presetTable[idx], idx
end

util.find = function(_array, _value)
    for i,v in ipairs(_array) do
        if v == _value then return i end
    end
    return nil
end

util.findAndRemove = function(_array, _value)
    local iter = util.find(_array, _value)
    if iter ~= nil then return table.remove(_array, iter) end
    return nil
end

util.makeSet = function(_tbl)
    local u = { }
    for _, v in ipairs(_tbl) do u[v] = true end
    return u
end

util.reverseInPlace = function(_array)
    local i, j = 1, #_array
    while i < j do
        _array[i], _array[j] = _array[j], _array[i]
        i = i + 1
        j = j - 1
    end
end

util.reverse = function(_array)
    local ret = {}
    local i = #_array
    local j = 1
    while i > 1 do
        ret[j] = _array[i]
        i = i - 1
        j = j + 1
    end
    return ret
end

util.shuffle = function(_array)
    local iterations = #_array
    local j
    
    for i = iterations, 2, -1 do
        j = math.ceil(random(0.1, i - 0.9))
        _array[i], _array[j] = _array[j], _array[i]
    end
end

util.reversePairs = function(_array)
    local function it(t,i)
        i=i-1
        local v=t[i]
        if v==nil then return v end
        return i,v
    end
  return it, _array, #_array+1
end

util.shallowCopy = function(_tbl)
    return {table.unpack(_tbl)}
end

return util
