#include "../ThorsChalice/DLLib.h"
#include "NisseHTTP/HTTPHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L5Plugin: public ThorsAnvil::ThorsChalice::ChalicePlugin
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


extern "C" void* chaliceFunctionNotCorrectName()
{
    return dynamic_cast<ThorsAnvil::ThorsChalice::ChalicePlugin*>(&l5);
}
