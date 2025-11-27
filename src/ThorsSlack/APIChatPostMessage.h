#ifndef THORSANVIL_SLACK_API_CHAT_POST_MESSAGE_H
#define THORSANVIL_SLACK_API_CHAT_POST_MESSAGE_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

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

struct PostMessageReply: public API::Reply
{
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

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::PostMessageReply, channel, ts, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessage, channel, text, blocks, icon_emoji, username, thread_ts);

#endif
