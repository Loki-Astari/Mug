#include "../ThorsMug/DLLib.h"
#include "NisseHTTP/HTTPHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L5Plugin: public ThorsAnvil::ThorsMug::MugPlugin
{
    void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(501);
    }
    public:
        virtual void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& /*name*/) override
        {
            handler.addPath(ThorsAnvil::Nisse::HTTP::Method::POST, "/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);});
        }
};

L5Plugin    l5;


extern "C" void* mugFunctionNotCorrectName()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&l5);
}
