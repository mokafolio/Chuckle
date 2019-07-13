// #include <Stick/ArgumentParser.hpp>
#include <Stick/FileUtilities.hpp>
#include <Stick/Path.hpp>

#include <Chuckle/ChuckleLuaBindings.hpp>
#include <Chuckle/ImGuiLuaBindings.hpp>
#include <ChuckleCore/ChuckleCore.hpp>

#define RETURN_ON_ERR(_err)                                                                        \
    if (_err)                                                                                      \
    {                                                                                              \
        printf("Error: %s\n", _err.message().cString());                                           \
        return EXIT_FAILURE;                                                                       \
    }

using namespace stick;

int main(int _argc, const char * _args[])
{
    // ArgumentParser parser("Chuckle");
    // RETURN_ON_ERR(parser.addArgument("-s", "--script", 1, false));
    // RETURN_ON_ERR(parser.parse(_args, _argc));

    {
        if (_argc < 2)
        {
            std::fprintf(stderr, "No script provided\n");
            return EXIT_FAILURE;
        }

        sol::state lua;
        lua.open_libraries(sol::lib::base,
                           sol::lib::string,
                           sol::lib::package,
                           sol::lib::os,
                           sol::lib::math,
                           sol::lib::io,
                           sol::lib::table);
        chuckle::registerLuaBindings(lua);
        chuckle::registerImGuiBindings(lua);

        String dirName = chuckle::executableDirectoryName();
        String packagePath("package.path = package.path .. ';");
        packagePath.append(dirName);
        packagePath.append("/../Scripts/?.lua;");
        packagePath.append(dirName);
        packagePath.append("/../Scripts/?/init.lua;");
        packagePath.append(dirName);
        packagePath.append("/../Scripts/Deps/?.lua;");
        packagePath.append(dirName);
        packagePath.append("/../Scripts/Deps/?/init.lua'");

        lua.script(packagePath.cString());

        auto tbl = lua.create_table();
        for (Size i = 2; i < _argc; ++i)
            tbl.add(_args[i]);

        lua["__args"] = tbl;

        lua.script(loadTextFile(_args[1]).ensure().cString());
    }

    return EXIT_SUCCESS;
}
