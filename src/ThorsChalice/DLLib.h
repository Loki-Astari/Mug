#ifndef THORSANVIL_THORS_CHALICE_DLLIB_H
#define THORSANVIL_THORS_CHALICE_DLLIB_H

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>
#include <string>

#include <cstddef>
#include <filesystem>


namespace FS = std::filesystem;


extern "C"
{
    typedef void(*GenericFuncPtr)();
    typedef GenericFuncPtr(*ChaliceFunc)();
}

typedef void (*ChaliceHanlde)(ThorsAnvil::Nisse::HTTP::Request&, ThorsAnvil::Nisse::HTTP::Response&);

namespace ThorsAnvil::ThorsChalice
{

enum CheckState {NoChange, ChangedButLocked, ChangeAndSwapped};


class DLLib
{
    FS::path                    path;
    FS::file_time_type          lastModified;
    void*                       lib             = nullptr;
    ChaliceHanlde               chaliceHandle   = nullptr;
    std::size_t                 activeCalls     = 0;
    bool                        reloadInProgress= false;
    bool                        loadFailed      = false;
    std::mutex                  mutex;
    std::condition_variable     cond;

    private:
        void inc();
        void dec();
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

        void call(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
        CheckState check();
        void checkWithForce();
};

class DLLibMap
{
    std::map<std::string, std::size_t>  libNameMap;
    std::vector<DLLib>                  loadedLibs;
    public:
    std::size_t load(std::string const& path);
    void        call(std::size_t index, ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response) {loadedLibs[index].call(request, response);}
    void        check(std::size_t index)    {loadedLibs[index].check();}
    void        checkAll()                  {for (auto& lib: loadedLibs){lib.check();}}
};

}

#endif
