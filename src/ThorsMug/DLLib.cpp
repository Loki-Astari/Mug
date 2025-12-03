#include "DLLib.h"

#include "ThorsLogging/ThorsLogging.h"
#include <dlfcn.h>
#include <filesystem>

using namespace ThorsAnvil::ThorsMug;


DLLib::DLLib()
{}

DLLib::DLLib(FS::path const& path)
    : path(path)
{
    if (path.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "DLLib", "Empty path used for library");
    }

    reload();
}

DLLib::DLLib(DLLib&& move) noexcept
{
    swap(move);
}

DLLib& DLLib::operator=(DLLib&& move) noexcept
{
    swap(move);
    return *this;
}

void DLLib::swap(DLLib& other) noexcept
{
    using std::swap;
    swap(path,          other.path);
    swap(lastModified,  other.lastModified);
    swap(lib,           other.lib);
    swap(plugin,        other.plugin);
}

char const* DLLib::safeDLerror()
{
    char const* message = dlerror();
    return message == nullptr ? "" : message;
}

DLLib::~DLLib()
{
    if (lib) {
        int status = dlclose(lib);
        if (status != 0) {
            ThorsLogError("DLLib", "~DLLib", "dlclose() failed: ", safeDLerror());
        }
    }
}

void DLLib::registerHandlers(NisHttp::HTTPHandler& handler, std::string const& name)
{
    if (plugin == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "registerHandlers", "Calling unloaded fuction");
    }
    plugin->registerHandlers(handler, name);
}

bool DLLib::check()
{
    return !(path.empty() || lastModified == FS::last_write_time(path));
}

void DLLib::reload()
{
    ThorsLogDebug("DLLib", "reload", "Reload DLL: ", path);
    std::error_code ec;
    lib = dlopen(FS::canonical(path, ec).c_str(), RTLD_NOW | RTLD_LOCAL);
    if (lib == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "reload", "dlopen() failed: ", safeDLerror());
    }

    void*           mugFuncSym = dlsym(lib, "mugFunction");
    if (mugFuncSym == nullptr) {
        dlclose(lib);
        lib = nullptr;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "reload", "dlsym() failed: ", safeDLerror());
    }
    lastModified = FS::last_write_time(path);


    MugFunc     mugFunc     = reinterpret_cast<MugFunc>(mugFuncSym);
    void*       mugPluginV  = (*mugFunc)();
    plugin                      = reinterpret_cast<MugPlugin*>(mugPluginV);
}

std::size_t DLLibMap::load(std::string const& path)
{
    std::error_code ec;
    FS::path    libPath = FS::canonical(path, ec);
    if (libPath.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLibMap", "load", "Invalid path to shared library: ", path);
    }
    auto find = libNameMap.find(libPath.string());
    if (find != std::end(libNameMap)) {
        return find->second;
    }
    std::size_t result = loadedLibs.size();
    libNameMap[libPath.string()] = result;
    loadedLibs.emplace_back(libPath);
    return result;
}
