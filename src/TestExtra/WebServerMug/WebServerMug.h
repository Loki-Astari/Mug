#ifndef THORSANVIL_THORS_MUG_WEB_SERVER_H
#define THORSANVIL_THORS_MUG_WEB_SERVER_H

#include "WebServerMugConfig.h"
#include "ThorsMug/MugPlugin.h"

#include <string>
#include <filesystem>

namespace ThorsAnvil::ThorsMug::WebServer
{

class WebServerPlugin: public ThorsAnvil::ThorsMug::MugPlugin
{
    std::filesystem::path const contentDir;

    public:
        WebServerPlugin(std::string const& config);
        virtual void initPlugin(NisHttp::HTTPHandler& handler) override;
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override;

        void handleRequestPath(NisHttp::Request& request, NisHttp::Response& response);
};

}

#endif
