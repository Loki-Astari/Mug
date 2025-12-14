#ifndef THORSANVIL_THORS_MUG_MUG_CONFIG_H
#define THORSANVIL_THORS_MUG_MUG_CONFIG_H

#include "ThorsMugConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <string>
#include <optional>
#include <vector>


namespace ThorsAnvil::ThorsMug
{

struct Plugin
{
    std::string                 pluginPath;    // The path to the shared library.
    std::string                 configPath;    // The path to config for this instance.
};

// A Port config is a port with optional SSL certificate to listen to.
// This has a list of plugins that are registered for this port.
// Each plugin is dynamically loaded at runtime.
struct PortConfig
{
    int                         port;           // Port to listen on
    std::optional<std::string>  certPath;       // The location of the SSL Certification (if any)
    std::vector<Plugin>         actions;        // Plugins that will be passed an ThorsAnvil::NissaHttp::HTTPHandler object
};

// The Mug server configuration.
struct MugConfig
{
    int                         controlPort         = 8079;     // Can send a stop command to this port.
    int                         libraryCheckTime    = 0;        // How often to check for library updates (mill-seconds). If 0 no checks.
    std::vector<PortConfig>     servers;                        // Ports with actions to load.
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::Plugin, pluginPath, configPath);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::PortConfig, port, certPath, actions);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::MugConfig, servers, controlPort, libraryCheckTime);

#endif
