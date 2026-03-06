#ifndef THORSANVIL_THORS_MUG_DLLIB_H
#define THORSANVIL_THORS_MUG_DLLIB_H

#include "ThorsMugConfig.h"
#include "MugConfig.h"
#include "ThorsMug/MugPlugin.h"
#include "ThorSerialize/Traits.h"
#include "NisseHTTP/HTTPHandler.h"

#include <vector>
#include <string>
#include <functional>
#include <filesystem>


namespace ThorsAnvil::ThorsMug
{

using ThorsAnvil::Nisse::HTTP::HTTPHandler;

class DLLib
{
    using HandlerRef = std::reference_wrapper<HTTPHandler>;
    struct Config
    {
        HandlerRef      handler;        // Nisse part to handle each request.
        std::string     config;         // Config string.
        MugPlugin*      plugin;         // result of calling mugFunc()
                                        // This is a Non owned pointer.
                                        // Any resources allocated should be deallocated by
                                        // calling ::dlcose() on the owned plugin.
    };

    std::filesystem::path               path;                       // Path to the dynamic library
    std::filesystem::file_time_type     lastModified;               // last time the library was modified.
    void*                               lib             = nullptr;  // Pointer to the loaded library.
    MugFunc                             mugFunc;                    // Function retrieved from the library
    std::vector<Config>                 instances;                  // Config object for each result of calling
                                                                    //mugFunc()

    public:
        DLLib& operator=(DLLib const&)  = delete;
        DLLib(DLLib&& move)             = delete;
        DLLib& operator=(DLLib&&)       = delete;

        DLLib(std::filesystem::path const&);
        DLLib(DLLib const&);
        ~DLLib();
        void swap(DLLib& other) noexcept;

        // These two functions are called from
        // DLLibMap.
        //      addInstance() is called when we load the config file.
        //      Then check() is called periodically to see if the DLL
        //      has been re-built.
        void addInstance(HTTPHandler& handler, Plugin const& name);
        bool check();
    private:
        void load();
        void unload();

        void loadLibrary();
        void unloadLibrary();

        static char const* safeDLerror();

};

}

#endif
