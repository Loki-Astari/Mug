#ifndef THORSANVIL_SLACK_API_H
#define THORSANVIL_SLACK_API_H

#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
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

struct BotIcon
{
    std::string                 image_36;
    std::string                 image_48;
    std::string                 image_72;
};

struct BotProfile
{
    std::string                 id;
    std::string                 app_id;
    std::string                 user_id;
    std::string                 name;
    BotIcon                     icons;
    bool                        deleted;
    std::time_t                 updated;
    std::string                 team_id;
};

struct Message
{
    std::string                 user;
    std::string                 type;
    std::string                 ts;
    std::string                 bot_id;
    std::string                 app_id;
    std::string                 text;
    std::string                 team;
    BotProfile                  bot_profile;
    BlockKit::Blocks            blocks;
    OptVecString                pinned_to;
    OptString                   permalink;
};
using OptMessage = std::optional<Message>;

struct ResponseMetadata
{
    VecString                   messages;
};
using OptResponseMetaData = std::optional<ResponseMetadata>;

struct Reply
{
    bool                        ok;
    OptString                   error;
    OptVecString                errors;
    OptString                   warning;
    OptVecString                warnings;
    OptResponseMetaData         response_metadata;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks, pinned_to, permalink);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::ResponseMetadata, messages);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reply, ok, error, errors, warning, warnings, response_metadata);


#endif
