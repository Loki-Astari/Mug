#ifndef THORSANVIL_THORS_MUG_MUG_CONFIG_H
#define THORSANVIL_THORS_MUG_MUG_CONFIG_H

#include <string>
#include <vector>
#include <filesystem>

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

namespace FS = std::filesystem;

namespace ThorsAnvil::ThorsMug
{

enum class ActionType {File, Lib};


struct Action
{
    ActionType                  type;
    std::string                 rootDir;
    std::string                 path;
    // For File:
    //      rootDir:        The directory on the file system where files will be loaded from.
    //      path:           registered with Nisse to match against input request.
    //                      Will match requests of the form:
    //                          http://Hostname/<path>/{FilePath}
    //
    //          The file loaded from the server will be "<rootDir>/<FilePath>" (after normalization)
    //          Note: If path tries to dip below root (using ../../ etc) it will log an error and return a 400
    //
    //          Example:
    //              If
    //                  path = "/files"
    //                  rootDir = "/etc/pages"
    //              Then
    //                  GET http://localhost:8080/files/mydata/file.html
    //              Will retrieve:
    //                  /etc/pages/mydata/file.html
    // For Lib:
    //      rootDir:        The path to the shared library.
    //      path:           registered with Nisse to match against input request.
};

struct PortConfig
{
    int                         port;
    std::optional<std::string>  certPath;
    std::vector<Action>         actions;
};

struct MugConfig
{
    int                         controlPort         = 8079;
    int                         libraryCheckTime    = 0;
    std::vector<PortConfig>     servers;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::Action, type, rootDir, path);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::PortConfig, port, certPath, actions);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsMug::MugConfig, servers, controlPort, libraryCheckTime);

#endif
