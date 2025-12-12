#ifndef THORSANVIL_THORS_MUG_DLLIB_H
#define THORSANVIL_THORS_MUG_DLLIB_H

#include "ThorsMugConfig.h"
#include "ThorsMug/MugPlugin.h"

#include "NisseHTTP/HTTPHandler.h"

#include <map>
#include <vector>
#include <string>
#include <cstddef>
#include <filesystem>


namespace FS        = std::filesystem;
namespace NisHttp   = ThorsAnvil::Nisse::HTTP;


namespace ThorsAnvil::ThorsMug
{

class DLLib
{
    FS::path                    path;
    FS::file_time_type          lastModified;
    void*                       lib             = nullptr;
    MugPlugin*                  plugin          = nullptr;
    using HandlerRef = std::reference_wrapper<NisHttp::HTTPHandler>;
    using Config     = std::pair<HandlerRef, std::string>;
    std::vector<Config>         configs;

    private:
        void load();
        void unload();
        void loadOnly();
        void unloadOnly();
        static char const* safeDLerror();

    public:
        DLLib(FS::path const& path);
        DLLib(DLLib const&)             = delete;
        DLLib& operator=(DLLib const&)  = delete;
        DLLib(DLLib&& move)             = delete;
        DLLib& operator=(DLLib&&)       = delete;
        ~DLLib();

        bool check();
        void init(NisHttp::HTTPHandler& handler, std::string const& configPath);
};

class DLLibMap
{
    std::map<std::string, DLLib>  libs;
    public:
        void    load(NisHttp::HTTPHandler& handler, std::string const& pluginPath, std::string const& configPath);
        void    checkAll()                                                          {for (auto& lib: libs){lib.second.check();}}
};

}

#endif
