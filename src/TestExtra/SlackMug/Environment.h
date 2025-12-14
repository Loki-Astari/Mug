#ifndef THORSANVIL_SLACKCLI_ENVIRONMENT_H
#define THORSANVIL_SLACKCLI_ENVIRONMENT_H

#include "SlackMugConfig.h"
#include "ThorSerialize/Traits.h"
#include <string>

struct Environment
{
    std::string     botToken;
    std::string     userToken;
    std::string     slackSecret;
    Environment(std::string const& fileName);
};
ThorsAnvil_MakeTrait(Environment, botToken, userToken, slackSecret);

#endif
