#ifndef THORSANVIL_SLACK_API_CHAT_H
#define THORSANVIL_SLACK_API_CHAT_H


#include "ThorsSlackConfig.h"
#include "API.h"
#include "SlackBlockKit.h"

namespace ThorsAnvil::Slack::API::Chat
{

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
};
using OptMessage = std::optional<Message>;

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks);

#endif
