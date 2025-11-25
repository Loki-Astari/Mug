#ifndef THORSANVIL_SLACK_API_CHAT_H
#define THORSANVIL_SLACK_API_CHAT_H


#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <optional>
#include <string>
#include <vector>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.postmessage
#if 0
// Example Reply
{
    "ok": true,
    "channel": "C09RU2URYMS",
    "ts": "1763354325.527129",
    "message":
    {
        "user": "U09S3D8R00Z",
        "type": "message",
        "ts": "1763354325.527129",
        "bot_id": "B09RJ4A000K",
        "app_id": "A09RQFXSEKC",
        "text": "I see: Time",
        "team": "T095XJHH589",
        "bot_profile":
        {
            "id": "B09RJ4A000K",
            "app_id": "A09RQFXSEKC",
            "user_id": "U09S3D8R00Z",
            "name": "ThorsSlackBotOne",
            "icons":
            {
                "image_36": "https://a.slack-edge.com/80588/img/plugins/app/bot_36.png",
                "image_48": "https://a.slack-edge.com/80588/img/plugins/app/bot_48.png",
                "image_72": "https://a.slack-edge.com/80588/img/plugins/app/service_72.png"
            },
            "deleted": false,
            "updated": 1762644664,
            "team_id": "T095XJHH589"
        },
        "blocks":
        [
            {
                "type": "rich_text",
                "block_id": "aRB4i",
                "elements":
                [
                    {
                        "type": "rich_text_section",
                        "elements":
                        [
                            {
                                "type": "text",
                                "text": "I see: Time"
                            }
                        ]
                    }
                ]
            }
        ]
    }
}
#endif

using OptString         = std::optional<std::string>;
using VecString         = std::vector<std::string>;
using OptVecString      = std::optional<VecString>;
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

struct ResponseMetadata
{
    VecString                   messages;
};
using OptResponseMetaData = std::optional<ResponseMetadata>;

struct PostMessageReply
{
    bool                        ok;
    OptString                   error;
    OptVecString                errors;
    OptString                   warning;
    OptVecString                warnings;
    OptResponseMetaData         response_metadata;
    OptString                   channel;
    OptString                   ts;
    OptMessage                  message;
};

struct PostMessage
{
    static constexpr char const* api = "https://slack.com/api/chat.postMessage";
    static constexpr bool hasBody = true;
    using Reply = PostMessageReply;
    std::string                 channel;
    OptString                   text;
    BlockKit::OptBlocks         blocks;
    OptString                   icon_emoji;
    OptString                   username;
    OptString                   thread_ts;
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::ResponseMetadata, messages);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessageReply, ok, error, errors, warning, warnings, response_metadata, channel, ts, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessage, channel, text, blocks, icon_emoji, username, thread_ts);

#endif
