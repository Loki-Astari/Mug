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

    loadOnly();
    // Expect DLLibMap::load() to call init() separately.
    // This is because one instance is created for all configured values.
    // But each instance is initialized with its own configuration file.
}

char const* DLLib::safeDLerror()
{
    char const* message = ::dlerror();
    return message == nullptr ? "" : message;
}

DLLib::~DLLib()
{
    unload();
}

bool DLLib::check()
{
    std::filesystem::file_time_type modifyTime = std::filesystem::last_write_time(path);
    if (modifyTime > lastModified) {
        lastModified = modifyTime;
        unload();
        load();
        return true;
    }
    return false;
}

void DLLib::load()
{
    loadOnly();
    for (auto& config: configs) {
        std::cerr << "Reload: " << config.second << "\n";
        ThorsLogDebug("DLLib", "load", "initPlugin ", config.second);
        plugin->initPlugin(config.first, config.second);
    }
}

void DLLib::unload()
{
    for (auto& config: configs) {
        ThorsLogDebug("DLLib", "unload", "destPlugin ", config.second);
        plugin->destPlugin(config.first);
    }
    unloadOnly();
}

void DLLib::loadOnly()
{
    ThorsLogDebug("DLLib", "loadOnly", "Reload DLL: ", path);
    std::error_code ec;
    lib = ::dlopen(std::filesystem::canonical(path, ec).c_str(), RTLD_NOW | RTLD_LOCAL | DLOPEN_PLAT_FLAG);
    if (lib == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadOnly", "dlopen() failed: ", safeDLerror());
    }

    void*           mugFuncSym = ::dlsym(lib, "mugFunction");
    if (mugFuncSym == nullptr) {
        ::dlclose(lib);
        lib = nullptr;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadOnly", "dlsym() failed: ", safeDLerror());
    }
    lastModified = std::filesystem::last_write_time(path);


    MugFunc     mugFunc     = reinterpret_cast<MugFunc>(mugFuncSym);
    void*       mugPluginV  = (*mugFunc)();
    plugin                  = reinterpret_cast<MugPlugin*>(mugPluginV);
    plugin->spinUp();
}

void DLLib::unloadOnly()
{
    plugin->spinDown();
    int status = ::dlclose(lib);
    if (status != 0) {
        ThorsLogError("DLLib", "unloadOnly", "dlclose() failed: ", safeDLerror());
    }
}


void DLLib::init(NisHttp::HTTPHandler& handler, std::string const& configPath)
{
    ThorsLogDebug("DLLib", "init", "Called ", plugin);
    configs.emplace_back(handler, configPath);
    plugin->initPlugin(handler, configPath);
}

void DLLibMap::load(NisHttp::HTTPHandler& handler, std::string const& pluginPath, std::string const& configPath)
{
    ThorsLogDebug("DLLibMap", "load ", pluginPath, " : ", configPath);
    std::error_code ec;
    std::filesystem::path    libPath = std::filesystem::canonical(pluginPath, ec);
    if (libPath.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLibMap", "load", "Invalid path to shared library: ", pluginPath);
    }
    // Note the plugin may have already been loaded.
    //      If it has we will not create a new object but reuse the DLLObject.
    //      Thus not call spinUp() again.
    auto inserted = libs.emplace(libPath.string(), libPath);
    inserted.first->second.init(handler, configPath);
}
