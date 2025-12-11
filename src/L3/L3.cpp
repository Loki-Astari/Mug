#include "../ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <thread>

class L3Plugin: public ThorsAnvil::ThorsMug::MugPlugin
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
        virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& /*name*/) override
        {
            handler.addPath("/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);return true;});
        }
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override
        {
            handler.remPath("/Plop/{Command}");
        }
};

L3Plugin    l3;


extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&l3);
}
