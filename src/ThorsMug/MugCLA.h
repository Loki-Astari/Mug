#ifndef THORSANVIL_THORS_MUG_MUG_CLA_H
#define THORSANVIL_THORS_MUG_MUG_CLA_H

#include "MugArgs.h"

#include <string_view>
#include <vector>
#include <map>
#include <filesystem>
#include <iostream>

#include "ThorsLogging/ThorsLogging.h"

namespace ThorsAnvil::ThorsMug
{

struct SplitArg
{
    std::string_view    flag;
    std::string_view    value;
    bool                hasValue;
};

using Paths         = std::vector<FS::path>;
using VerbosityMap  = std::map<std::string_view, loguru::Verbosity>;

class MugCLA
{
    static const Paths          searchPath;
    static const VerbosityMap   verbosity;

    MugCLAInterface&        args;
    bool                        setConfig = false;

    SplitArg    splitArgument(std::string_view arg);
    void        parseArguments(std::vector<std::string_view> const& arguments);
    public:
        MugCLA(std::vector<std::string_view> const& arguments, MugCLAInterface& args);

        void displayHelp(std::string_view command, std::ostream& output = std::cout);
};

}

#endif
