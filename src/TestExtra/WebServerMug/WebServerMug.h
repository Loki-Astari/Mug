#ifndef THORSANVIL_THORS_MUG_WEB_SERVER_H
#define THORSANVIL_THORS_MUG_WEB_SERVER_H

#include "WebServerMugConfig.h"
#include "ThorsMug/MugPlugin.h"

#include <string>
#include <filesystem>

namespace ThorsAnvil::ThorsMug::WebServer
{

class WebServerPlugin: public ThorsAnvil::ThorsMug::MugPluginSimple
{
    std::filesystem::path const contentDir;

    public:
        WebServerPlugin(std::string const& config);
        virtual std::vector<ThorsAnvil::ThorsMug::Action> getAction() override;

        void handleRequestPath(NisHttp::Request const& request, NisHttp::Response& response);
};

}

#endif
