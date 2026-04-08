#ifndef THORSANVIL_THORS_MUG_DLLIBMAP_H
#define THORSANVIL_THORS_MUG_DLLIBMAP_H

#include "ThorsMugConfig.h"
#include "DLLib.h"

#include <map>
#include <filesystem>

namespace ThorsAnvil::ThorsMug
{

using ThorsAnvil::Nisse::HTTP::HTTPHandler;

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

#if defined(THORSMUG_HEADER_ONLY) && THORSMUG_HEADER_ONLY == 1
#include "DLLibMap.source"
#endif

#endif
