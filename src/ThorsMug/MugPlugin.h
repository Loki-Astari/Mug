#ifndef THORSANVIL_THORS_MUG_MUG_PLUGIN_H
#define THORSANVIL_THORS_MUG_MUG_PLUGIN_H

#include "ThorsMugConfig.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseHTTP/HTTPHandler.h"

#include <string>


namespace NisHttp   = ThorsAnvil::Nisse::HTTP;


extern "C"
{
    typedef void*(*MugFunc)(char const* config);
}

typedef void (*MugHanlde)(ThorsAnvil::Nisse::HTTP::Request&, ThorsAnvil::Nisse::HTTP::Response&);

namespace ThorsAnvil::ThorsMug
{

class MugPlugin
{
    public:
        virtual ~MugPlugin()                                    {}
        virtual void initPlugin(NisHttp::HTTPHandler& handler)  = 0;
        virtual void destPlugin(NisHttp::HTTPHandler& handler)  = 0;
};

}

#endif
