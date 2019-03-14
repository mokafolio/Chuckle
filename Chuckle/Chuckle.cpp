#include <Stick/ArgumentParser.hpp>
#include <Stick/FileUtilities.hpp>
#include <Stick/Path.hpp>

#include <ChuckleCore/ChuckleCore.hpp>
#include <Chuckle/ChuckleLuaBindings.hpp>
#include <Chuckle/ImGuiLuaBindings.hpp>

// #include <whereami.h>

#define RETURN_ON_ERR(_err)                                                                        \
    if (_err)                                                                                      \
    {                                                                                              \
        printf("Error: %s\n", _err.message().cString());                                           \
        return EXIT_FAILURE;                                                                       \
    }

using namespace stick;

// static String executablePath()
// {
//     int length = wai_getExecutablePath(NULL, 0, NULL);
//     String ret;
//     ret.resize(length);
//     wai_getExecutablePath((char *)ret.ptr(), length, NULL);
//     return ret;
// }

int main(int _argc, const char * _args[])
{
    ArgumentParser parser("Chuckle");
    RETURN_ON_ERR(parser.addArgument("-s", "--script", 1, false));
    RETURN_ON_ERR(parser.parse(_args, _argc));

    sol::state lua;
    lua.open_libraries(sol::lib::base,
                       sol::lib::string,
                       sol::lib::package,
                       sol::lib::os,
                       sol::lib::math,
                       sol::lib::table);
    chuckle::registerLuaBindings(lua);
    chuckle::registerImGuiBindings(lua);

    String dirName = chuckle::executableDirectoryName();
    String packagePath("package.path = package.path .. ';");
    packagePath.append(dirName);
    packagePath.append("/../Scripts/?.lua;");
    packagePath.append(dirName);
    packagePath.append("/../Scripts/?/init.lua'");
    
    lua.script(packagePath.cString());

    lua.script(loadTextFile(parser.get<String>("script")).ensure().cString());
    return EXIT_SUCCESS;
}
