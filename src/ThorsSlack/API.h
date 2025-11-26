#ifndef THORSANVIL_SLACK_API_H
#define THORSANVIL_SLACK_API_H

#include "SlackBlockKit.h"
#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <string>
#include <vector>
#include <optional>

namespace ThorsAnvil::Slack::API
{

using OptBool       = std::optional<bool>;
using OptInt        = std::optional<int>;
using OptString     = std::optional<std::string>;
using VecString     = std::vector<std::string>;
using OptVecString  = std::optional<std::vector<std::string>>;

struct Reply
{
    bool                        ok;
    OptString                   error;
    OptVecString                errors;
    OptString                   warning;
    OptVecString                warnings;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reply, ok, error, errors, warning, warnings);


#endif
