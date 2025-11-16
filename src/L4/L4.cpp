#include "../ThorsChalice/DLLib.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L4Plugin: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
    void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(404);
    }
    public:
        virtual void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& /*name*/) override
        {
            handler.addPath("/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);});
        }
};

L4Plugin    l4;


extern "C" void* chaliceFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsChalice::ChalicePlugin*>(&l4);
}
