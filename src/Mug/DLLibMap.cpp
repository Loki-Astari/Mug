#include "DLLibMap.h"

#include "ThorsLogging/ThorsLogging.h"

using namespace ThorsAnvil::ThorsMug;


void DLLibMap::load(NisHttp::HTTPHandler& handler, Plugin const& pluginInfo)
{
    ThorsLogInfo("DLLibMap", "load ", pluginInfo.pluginPath, " : ", pluginInfo.config.getString());
    std::error_code ec;
    std::filesystem::path    libPath = std::filesystem::canonical(pluginInfo.pluginPath, ec);
    if (libPath.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLibMap", "load", "Invalid path to shared library: ", pluginInfo.pluginPath);
    }
    // Note the plugin may have already been loaded.
    //      If it has we will not create a new object but reuse the DLLObject.
    Iterator find = libs.find(libPath.string());
    if (find == std::end(libs)) {
        auto insert = libs.emplace(libPath, libPath);
        find = insert.first;
    }
    find->second.addInstance(handler, pluginInfo);
}
