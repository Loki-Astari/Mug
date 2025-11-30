#ifndef THORSANVIL_THORSSLACK_TEST_ENVIRONMENT_H
#define THORSANVIL_THORSSLACK_TEST_ENVIRONMENT_H

#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"

#include <string>
#include <fstream>

struct Environment
{
    std::string     botToken;
    std::string     userToken;
    std::string     slackSecret;
    Environment(std::string const& fileName)
    {
        std::ifstream   file(fileName);
        file >> ThorsAnvil::Serialize::jsonImporter(*this);
    }
};
ThorsAnvil_MakeTrait(Environment, botToken, userToken, slackSecret);

#endif
