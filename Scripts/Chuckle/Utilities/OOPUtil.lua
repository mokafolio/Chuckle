local util = {}

util.functor = function(_obj, _fn)
    return setmetatable({}, {
        __call = function(...) 
            -- using select to skip the proxy table 
            return _fn(_obj, select(2, ...)) 
    end })
end

return util
