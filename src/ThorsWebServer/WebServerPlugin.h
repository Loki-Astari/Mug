#ifndef THORSANVIL_THORS_MUG_WEB_SERVER_H
#define THORSANVIL_THORS_MUG_WEB_SERVER_H

#include "ThorsWebServerConfig.h"
#include "ThorsMug/MugPlugin.h"

#include <string>
#include <filesystem>

namespace ThorsAnvil::ThorsMug::WebServer
{

class WebServerPlugin: public ThorsAnvil::ThorsMug::MugPlugin
{
    public:
        virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& config) override;
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override;

        void handleRequestPath(NisHttp::Request& request, NisHttp::Response& response, std::filesystem::path const& contentDir);
};

}

#endif
