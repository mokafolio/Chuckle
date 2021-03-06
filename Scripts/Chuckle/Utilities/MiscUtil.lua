local tbl = require("Chuckle.Utilities.TableUtil")
local util = {}

util.promote = function(_self, _toTable)
    local toTbl = _toTable or _G
    for k, v in pairs(_self) do
        toTbl[k] = v
    end
    return _toTable
end

util.exitOnError = function(_err)
    -- @TODO: just checking for it being a table seems rather weak...
    -- while that will work if we assume that the error tables are only 
    -- returned from native functions returning usertypes on success, 
    -- this should be more bullet proof.
    -- 
    -- @TODO: possibly replace this with an alternative mechanism using luas pcall/error functions
    if _err and type(_err) == "table" then
        print("Error: ", _err.message)
        os.exit()
    end
    -- if its not an error, we simply return the value that was passed in
    return _err
end

util.dateToTimestamp = function(_str, _pattern)
    local runyear, runmonth, runday, runhour, runminute, runseconds = _str:match(_pattern)
    return os.time({year = runyear, month = runmonth, day = runday, hour = runhour, min = runminute, sec = runseconds})
end

util.randomPick = function(_a, _b)
    local lower = _a
    local bigger = _b
    if lower.chance > bigger.chance then
        bigger, lower = lower, bigger
    end

    local lowpercentage = 1.0 / (bigger.chance / lower.chance + 1.0)
    if random(0, 1) <= lowpercentage then
        return lower
    end
    return bigger
end

util.maxInteger64 = function()
    return 0x7FFFFFFFFFFFFFFF
end

util.maxInteger32 = function()
    return 0x7FFFFFFF
end

util.defaultEpsilon = function()
    return 0.00001
end

util.floatEquals = function(_a, _b, _epsilon)
    return math.abs(_a - _b) < (_epsilon and _epsilon or util.defaultEpsilon())
end

util.encodeTwo4Bit = function(_a, _b)
    local ret = 0
    ret = ret | (_a & 0xF) << 4
    ret = ret | (_b & 0xF)
    return ret
end

util.decodeTwo4Bit = function(_int)
    return _int >> 4, _int & 0xF
end

local __hex = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"}
util.randomHexString = function(_length)
    if _length > 0 then
        return util.randomHexString(_length - 1) .. tbl.randomEntry(__hex)
    else
        return ""
    end
end

util.hexToRGB = function(_hex)
    local hex = _hex:gsub("#","")
    if hex:len() == 3 then
      return (tonumber("0x"..hex:sub(1,1))*17)/255, (tonumber("0x"..hex:sub(2,2))*17)/255, (tonumber("0x"..hex:sub(3,3))*17)/255
    else
      return tonumber("0x"..hex:sub(1,2))/255, tonumber("0x"..hex:sub(3,4))/255, tonumber("0x"..hex:sub(5,6))/255
    end
end

util.hexToColorRGBA = function(_hex)
    local r,g,b = util.hexToRGB(_hex)
    return ColorRGBA(r,g,b,1)
end

return util