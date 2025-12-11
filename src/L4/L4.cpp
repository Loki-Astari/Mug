#include "../ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L4Plugin: public ThorsAnvil::ThorsMug::MugPlugin
{
    void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(404);
    }
    public:
        virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& /*name*/) override
        {
            handler.addPath("/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);return true;});
        }
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override
        {
            handler.remPath("/Plop/{Command}");
        }
};

L4Plugin    l4;


extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&l4);
}
