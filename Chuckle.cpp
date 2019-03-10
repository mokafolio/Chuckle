#include <StickLuaSol/StickLuaSol.hpp>
#include <CrunchLuaSol/CrunchLuaSol.hpp>
#include <LukeLuaSol/LukeLuaSol.hpp>
#include <DabLuaSol/DabLuaSol.hpp>
#include <PaperLuaSol/PaperLuaSol.hpp> 
#include <Stick/FileUtilities.hpp>
#include <Stick/ArgumentParser.hpp>

#include <Paper2/Group.hpp>

#define RETURN_ON_ERR(_err)                                                                        \
    if (_err)                                                                                      \
    {                                                                                              \
        printf("Error: %s\n", _err.message().cString());                                           \
        return EXIT_FAILURE;                                                                       \
    }

using namespace stick;
namespace cl = crunchLuaSol;
namespace pl = paperLuaSol;
namespace ll = lukeLuaSol;
namespace dl = dabLuaSol;

Result<dab::RenderDevice*> fml()
{
    return Result<dab::RenderDevice*>();
}

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
    ll::registerLuke(lua);
    cl::registerCrunch(lua);
    dl::registerDab(lua);
    pl::registerPaper(lua);

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
