local path = require("Chuckle.Utilities.PathUtil")
--registers the native path util functions
__chuckle.registerPathUtil(path)

return
{
    math = require("Chuckle.Utilities.MathUtil"),
    conv = require("Chuckle.Utilities.ConversionUtil"),
    misc = require("Chuckle.Utilities.MiscUtil"),
    path = path,
    tbl = require("Chuckle.Utilities.TableUtil"),
    oop = require("Chuckle.Utilities.OOPUtil")
}
