local util = {}

util.toHex = function(_int, _minHexChars)
    return string.format(_minHexChars and "%0" .. math.floor(_minHexChars) .. "x" or "%x", _int)
end

util.hexToBinaryString = function(_str)
    local map = {
        ['0'] = '0000',
        ['1'] = '0001',
        ['2'] = '0010',
        ['3'] = '0011',
        ['4'] = '0100',
        ['5'] = '0101',
        ['6'] = '0110',
        ['7'] = '0111',
        ['8'] = '1000',
        ['9'] = '1001',
        ['a'] = '1010',
        ['b'] = '1011',
        ['c'] = '1100',
        ['d'] = '1101',
        ['e'] = '1110',
        ['f'] = '1111'
    }
    return _str:gsub('[0-9a-f]', map)
end

util.toDec = function(_hex)
    return tonumber(_hex, 16)
end

return util
