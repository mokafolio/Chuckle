#include <Stick/FileUtilities.hpp>
#include <Stick/ArgumentParser.hpp>
#include <Chuckle/ChuckleLuaBindings.hpp>

#define RETURN_ON_ERR(_err)                                                                        \
    if (_err)                                                                                      \
    {                                                                                              \
        printf("Error: %s\n", _err.message().cString());                                           \
        return EXIT_FAILURE;                                                                       \
    }

using namespace stick;

int main(int _argc, const char * _args[])
{
    printf("FUCK\n"); asffas
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

    lua.script("package.path = package.path .. ';../Scripts/?.lua'");
    lua.set_function("testResult", []()
    {
        return Result<Int32>(99);
    });
    lua.set_function("testResultFail", []()
    {
        return Result<Int32>(Error(ec::InvalidOperation, "fuck you", STICK_FILE, STICK_LINE));
    });
    lua.set_function("fml", fml);
    lua.script(loadTextFile(parser.get<String>("script")).ensure().cString());

    return EXIT_SUCCESS;
}
