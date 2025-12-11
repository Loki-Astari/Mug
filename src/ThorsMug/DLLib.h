#ifndef THORSANVIL_THORS_MUG_DLLIB_H
#define THORSANVIL_THORS_MUG_DLLIB_H

#include "ThorsMugConfig.h"
#include "MugPlugin.h"

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
    MugPlugin*              plugin          = nullptr;

    private:
        void reload();
        char const* safeDLerror();

    public:
        DLLib();
        DLLib(FS::path const& path);
        DLLib(DLLib const&)             = delete;
        DLLib& operator=(DLLib const&)  = delete;
        DLLib(DLLib&& move)             noexcept;
        DLLib& operator=(DLLib&&)       noexcept;
        void swap(DLLib& other)         noexcept;
        ~DLLib();

        void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& name);
        bool check();
};

class DLLibMap
{
    std::map<std::string, std::size_t>  libNameMap;
    std::vector<DLLib>                  loadedLibs;
    public:
    std::size_t load(std::string const& path);
    void        registerHandlers(std::size_t index, NisHttp::HTTPHandler& handler, std::string const& name)  {loadedLibs[index].registerHandlers(handler, name);}
    void        check(std::size_t index)                                            {loadedLibs[index].check();}
    void        checkAll()                                                          {for (auto& lib: loadedLibs){lib.check();}}
};

}

#endif
