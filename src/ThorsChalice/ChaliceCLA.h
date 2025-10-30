#ifndef THORSANVIL_THORS_CHALICE_CHALICE_CLA_H
#define THORSANVIL_THORS_CHALICE_CHALICE_CLA_H

#include <vector>
#include <string_view>
#include <filesystem>

#include "ThorSerialize/SerUtil.h"
#include "ThorSerialize/Traits.h"

namespace ThorsAnvil::ThorsChalice
{

enum class ActionType {File, Lib};
struct Action
{
    std::string         path;
    ActionType          type;
    std::string         value;
};
struct Config
{
    int                 port;
    std::vector<Action> actions;
};


struct SplitArg
{
    std::string_view    flag;
    std::string_view    value;
};
class ChaliceCLA
{
    static std::vector<std::filesystem::path>        searchPath;

    bool                        help = false;
    bool                        silent = false;
    std::filesystem::path       configPath;
    std::vector<Config>         config;

    SplitArg    splitArgument(std::string_view arg);
    void        parseArguments(std::vector<std::string_view> const& arguments);
    public:
        ChaliceCLA(std::vector<std::string_view> const& arguments);

        bool isHelpNeeded() const {return help;}
        void displayHelp(std::string_view command);
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Action, path, type, value);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Config, port, actions);

#endif
