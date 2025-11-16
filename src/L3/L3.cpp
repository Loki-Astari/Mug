#include "../ThorsChalice/DLLib.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <thread>

class L3Plugin: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
    void handle(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(305);
        auto sleepTime = request.variables()["sleep"];
        if (!sleepTime.empty()) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2000ms);
        }
    }
    public:
        virtual void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& /*name*/) override
        {
            handler.addPath("/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);});
        }
};

L3Plugin    l3;


extern "C" void* chaliceFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsChalice::ChalicePlugin*>(&l3);
}
