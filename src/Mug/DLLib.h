#ifndef THORSANVIL_THORS_MUG_DLLIB_H
#define THORSANVIL_THORS_MUG_DLLIB_H

#include "ThorsMugConfig.h"
#include "ThorsMug/MugPlugin.h"

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
    using Config     = std::pair<HandlerRef, std::string>;

    std::filesystem::path           path;
    std::filesystem::file_time_type lastModified;
    void*                           lib             = nullptr;
    MugPlugin*                      plugin          = nullptr;
    std::vector<Config>             configs;

    private:
        void load();
        void unload();
        void loadOnly();
        void unloadOnly();
        static char const* safeDLerror();

    public:
        DLLib(std::filesystem::path const& path);
        DLLib(DLLib const&)             = delete;
        DLLib& operator=(DLLib const&)  = delete;
        DLLib(DLLib&& move)             = delete;
        DLLib& operator=(DLLib&&)       = delete;
        ~DLLib();

        bool check();
        void init(HTTPHandler& handler, std::string const& configPath);
};

class DLLibMap
{
    std::map<std::string, DLLib>  libs;
    public:
        void    load(HTTPHandler& handler, std::string const& pluginPath, std::string const& configPath);
        void    checkAll()    {for (auto& lib: libs){lib.second.check();}}
};

}

#endif
