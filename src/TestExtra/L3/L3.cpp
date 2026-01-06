#include "../../ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <thread>

class L3Plugin: public ThorsAnvil::ThorsMug::MugPluginSimple
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
        virtual std::vector<ThorsAnvil::ThorsMug::Action> getAction() override
        {
            return {{ThorsAnvil::Nisse::HTTP::Method::GET, "/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);return true;}}};
        }
};

L3Plugin    l3;


extern "C" ThorsAnvil::ThorsMug::MugPlugin* mugCreateInstance(char const*)
{
    return &l3;
}
