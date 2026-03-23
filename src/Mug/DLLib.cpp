#include "DLLib.h"

#include "ThorsLogging/ThorsLogging.h"
#include <dlfcn.h>

using namespace ThorsAnvil::ThorsMug;

/*
 * We call:
 *      loadLibrary() / unloadLibrary() once for each DLL.
 *                                      This loads the library and makes sure we can get the "MugFunc"
 *                                      from the DLL.
 * For each config object we find we call:
 *      addInstance() which makes sure the library is correctly loaded (see above)
 *      Then calls the "MugFunc" passing the config to get a "Non Owned" pointer to the "MugPlugin" object.
 *
 *      Then we call "start()" on the MugPlugin object.
 *          This is usually used to register the handlers
 *
 * We call
 *      load() / unload() is used to unload and re-load each instance that has previously been created with
 *      addInstance(). This is done if we detect that the DLL has been updated.
 *
 * Note:
 *      unload() is also called on destruction to make sure "stop()" is called on the plugin.
 */
DLLib::DLLib(std::filesystem::path const& path)
    : path(path)
{
    if (path.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "DLLib", "Empty path used for library");
    }
    loadLibrary();
    // The loading of each config is done via: addInstance()
    // At this point there is nothing to load()
    // That is why the constructor and destructor are not symmetric.
}

// Used by the check() function to try and re-load and DLL.
DLLib::DLLib(DLLib const& copy)
    : DLLib(copy.path)
{
    // Note: We will need to call load()
    //       to reload all these configuration instances.
    for (auto& inst: copy.instances) {
        instances.emplace_back(inst.handler, inst.config, nullptr);
    }
}

DLLib::~DLLib()
{
    unload();
    unloadLibrary();
}

void DLLib::swap(DLLib& other) noexcept
{
    using std::swap;
    swap(path,              other.path);
    swap(lastModified,      other.lastModified);
    swap(lastLoadAttempt,   other.lastLoadAttempt);
    swap(lib,               other.lib);
    swap(mugFunc,           other.mugFunc);
    swap(instances,         other.instances);
}

// Check if the DLL has been re-built.
// If it has then we unload the DLL and re-load the new version.
bool DLLib::check()
{
    std::filesystem::file_time_type modifyTime = std::filesystem::last_write_time(path);
    if (modifyTime > lastLoadAttempt) {

        // Even if we fail to fail to load we want to track the last attempt.
        // There is no point in trying to reload if we know it is going to fail.
        lastLoadAttempt = modifyTime;

        // Try and load the new version.
        // Note: This may generate an exception.
        // We will catch log and drop any exception.
        try
        {
            {
                DLLib   reload(*this);
                // Swap the current and reloaded state.
                swap(reload);
                // The destructor will unload the current config.
            }
            // We just swapped the reload variable above with this.
            // We now need to load() the config file again as the destructor
            // called stop on each MugPlugin object to unregister any handlers.
            load();
            return true;
        }
        catch (std::exception const& e)
        {
            ThorsLogError("ThorsAnvil::ThorsMug::DLLib", "check", "Failed to load new version of the dynamic library: ", path.c_str(), " ", e.what());
        }
        catch (...)
        {
            ThorsLogError("ThorsAnvil::ThorsMug::DLLib", "check", "Failed to load new version of the dynamic library: ", path.c_str(), " Unknown");
        }
    }
    return false;
}

void DLLib::load()
{
    for (auto& instance: instances) {
        ThorsLogInfo("DLLib", "load", "Plugin Config: ", instance.config);
        instance.plugin         = mugFunc(instance.config.c_str());
        if (instance.plugin) {
            // Adds the handlers back.
            // Requests can now be handeled.
            instance.plugin->start(instance.handler);
        }
        else {
            ThorsLogError("DLLib", "load", "Plugin Config: failed to load a plugin: ", instance.config);
        }
    }
}
void DLLib::unload()
{
    for (auto& instance: instances) {
        ThorsLogInfo("DLLib", "unload", "Plugin Config: ", instance.config);
        if (instance.plugin) {
            // Removes the handlers
            // Any subsequent requests will not be handled.
            // Until the plugin is reloaded.
            //
            // Nisse makes sure that that in-flight requests are finished
            // before this function returns.
            instance.plugin->stop(instance.handler);
        }
    }
}

void DLLib::loadLibrary()
{
    ThorsLogInfo("DLLib", "loadLibrary", "path: ", path);
    std::error_code ec;
    lib = ::dlopen(std::filesystem::canonical(path, ec).c_str(), RTLD_NOW | RTLD_LOCAL | DLOPEN_PLAT_FLAG);
    if (lib == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadLibrary", "dlopen() failed: ", safeDLerror());
    }

    void*           mugFuncSym = ::dlsym(lib, "mugCreateInstance");
    if (mugFuncSym == nullptr) {
        ::dlclose(lib);
        lib = nullptr;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "loadLibrary", "dlsym() failed: ", safeDLerror());
    }
    lastModified = std::filesystem::last_write_time(path);
    lastLoadAttempt = lastModified;
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

void DLLib::addInstance(HTTPHandler& handler, Plugin const& pluginInfo)
{
    ThorsLogInfo("DLLib", "addInstance", "Called ", path.c_str(), " ", pluginInfo.config.getString());
    std::string config = pluginInfo.config.getString();
    MugPlugin*  pluginPtr = mugFunc(config.c_str());
    if (pluginPtr == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "addInstance", "mugFunc() returned nullptr!");
    }
    instances.emplace_back(handler, std::move(config), pluginPtr);

    // Add the handlers to the main loop.
    pluginPtr->start(handler);
}
