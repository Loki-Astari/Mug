#include "DLLib.h"

#include "ThorsLogging/ThorsLogging.h"
#include <dlfcn.h>

using namespace ThorsAnvil::ThorsMug;


DLLib::DLLib(std::filesystem::path const& path)
    : path(path)
{
    if (path.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "DLLib", "Empty path used for library");
    }
    loadLibrary();
}
DLLib::~DLLib()
{
    unload();
    //unloadLibrary();
}

void DLLib::load()
{
    for (auto& instance: instances) {
        ThorsLogInfo("DLLib", "load", "Plugin Config: ", instance.config);
        void*       plugin      = mugFunc(instance.config.c_str());
        instance.plugin         = reinterpret_cast<MugPlugin*>(plugin);
        instance.plugin->start(instance.handler);
    }
}
void DLLib::unload()
{
    for (auto& instance: instances) {
        ThorsLogInfo("DLLib", "unload", "Plugin Config: ", instance.config);
        instance.plugin->stop(instance.handler);
    }
}


void DLLib::addInstance(HTTPHandler& handler, Plugin const& pluginInfo)
{
    ThorsLogInfo("DLLib", "addInstance", "Called ", path.c_str(), " ", pluginInfo.config.getString());
    std::string config = pluginInfo.config.getString();
    void*       plugin = mugFunc(config.c_str());
    MugPlugin*  pluginPtr = reinterpret_cast<MugPlugin*>(plugin);
    instances.emplace_back(handler, std::move(config), pluginPtr);
    pluginPtr->start(handler);
}

void DLLib::loadLibrary()
{
    ThorsLogInfo("DLLib", "loadLibrary", "path: ", path);
    std::error_code ec;
    lib = ::dlopen(std::filesystem::canonical(path, ec).c_str(), RTLD_NOW | RTLD_LOCAL | DLOPEN_PLAT_FLAG);
    if (lib == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadLibrary", "dlopen() failed: ", safeDLerror());
    }

    void*           mugFuncSym = ::dlsym(lib, "mugFunction");
    if (mugFuncSym == nullptr) {
        ::dlclose(lib);
        lib = nullptr;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadLibrary", "dlsym() failed: ", safeDLerror());
    }
    lastModified = std::filesystem::last_write_time(path);
    mugFunc      = reinterpret_cast<MugFunc>(mugFuncSym);
}

void DLLib::unloadLibrary()
{
    ThorsLogInfo("DLLib", "unloadLibrary", "path: ", path);
    if (lib) {
        ::dlclose(lib);
    }
}

char const* DLLib::safeDLerror()
{
    char const* message = ::dlerror();
    return message == nullptr ? "" : message;
}


bool DLLib::check()
{
    std::filesystem::file_time_type modifyTime = std::filesystem::last_write_time(path);
    if (modifyTime > lastModified) {
        // Unload the plugins we have.
        unload();

        // Reload the DLL
        unloadLibrary();
        lib = nullptr;
        loadLibrary();

        // Reload the plugins we have configed.
        load();
        return true;
    }
    return false;
}

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
