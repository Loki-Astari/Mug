#ifndef THORSANVIL_THORS_MUG_MUG_ARGS_H
#define THORSANVIL_THORS_MUG_MUG_ARGS_H

#include "ThorsMugConfig.h"

#include "ThorsLogging/ThorsLogging.h"

#include <string_view>
#include <filesystem>


namespace ThorsAnvil::ThorsMug
{

struct MugCLAInterface
{
    virtual void setHelp()                                  = 0;
    virtual void setSilent()                                = 0;
    virtual void setConfigFile(std::filesystem::path file)  = 0;
    virtual void setSignal(std::string_view signal)         = 0;
    virtual void setPidFile(std::filesystem::path file)     = 0;
    virtual void logAddFile(std::filesystem::path file)     = 0;
    virtual void logAddSys(std::string_view app)            = 0;
    virtual void logSetLevel(loguru::Verbosity level)       = 0;
};

struct MugArgs: public MugCLAInterface
{
    bool        help    = false;
    bool        silent  = false;
    std::filesystem::path    configPath;
    std::string              signal;
    std::filesystem::path    pidFile = "/tmp/mug.pid";

    virtual void setHelp()                                  override;
    virtual void setSilent()                                override;
    virtual void setConfigFile(std::filesystem::path file)  override;
    virtual void setSignal(std::string_view signal)         override;
    virtual void setPidFile(std::filesystem::path file)     override;
    virtual void logAddFile(std::filesystem::path file)     override;
    virtual void logAddSys(std::string_view app)            override;
    virtual void logSetLevel(loguru::Verbosity level)       override;
};

}

#if defined(THORSMUG_HEADER_ONLY) && THORSMUG_HEADER_ONLY == 1
#include "MugArgs.source"
#endif

#endif
