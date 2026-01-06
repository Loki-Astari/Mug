#include "../../ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L4Plugin: public ThorsAnvil::ThorsMug::MugPluginSimple
{
    void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(404);
    }
    public:
        virtual std::vector<ThorsAnvil::ThorsMug::Action> getAction() override
        {
            return {{ThorsAnvil::Nisse::HTTP::Method::GET, "/Plop/{Command}",[&](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);return true;}}};
        }
};

L4Plugin    l4;


extern "C" MugPlugin* mugFunction(char const*)
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&l4);
}
