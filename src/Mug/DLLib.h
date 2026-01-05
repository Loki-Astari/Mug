#ifndef THORSANVIL_THORS_MUG_DLLIB_H
#define THORSANVIL_THORS_MUG_DLLIB_H

#include "ThorsMugConfig.h"
#include "MugConfig.h"
#include "ThorsMug/MugPlugin.h"
#include "ThorSerialize/Traits.h"
#include "NisseHTTP/HTTPHandler.h"

#include <map>
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
        HandlerRef      handler;
        std::string     config;
        MugPlugin*      plugin;
    };

    std::filesystem::path               path;
    std::filesystem::file_time_type     lastModified;
    void*                               lib             = nullptr;
    MugFunc                             mugFunc;
    std::vector<Config>                 instances;

    public:
        DLLib(DLLib const&)             = delete;
        DLLib& operator=(DLLib const&)  = delete;
        DLLib(DLLib&& move)             = delete;
        DLLib& operator=(DLLib&&)       = delete;

        DLLib(std::filesystem::path const&);
        ~DLLib();
        void addInstance(HTTPHandler& handler, Plugin const& name);
        bool check();
    private:
        void load();
        void unload();

        void loadLibrary();

        static char const* safeDLerror();

};

class DLLibMap
{
    using Container = std::map<std::filesystem::path, DLLib>;
    using Iterator = Container::iterator;

    Container   libs;
    public:
        void    load(HTTPHandler& handler, Plugin const& pluginInfo);
        void    checkAll()    {for (auto& lib: libs){lib.second.check();}}
};

}

#endif
