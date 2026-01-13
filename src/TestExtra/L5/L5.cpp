#include "../../ThorsMug/MugPlugin.h"
#include "NisseHTTP/HTTPHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class L5Plugin: public ThorsAnvil::ThorsMug::MugPluginSimple
{
    void handle(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
    {
        response.setStatus(501);
    }
    public:
        virtual std::vector<ThorsAnvil::ThorsMug::Action> getAction() override
        {
            return {{ThorsAnvil::Nisse::HTTP::Method::POST, "/Plop/{Command}", [&](ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response){handle(request, response);return true;}}};
        }
};

L5Plugin    l5;


extern "C" ThorsAnvil::ThorsMug::MugPlugin* mugFunctionNotCorrectName(char const*)
{
    return &l5;
}
