#ifndef THORSANVIL_SLACK_API_CHAT_UPDATE_H
#define THORSANVIL_SLACK_API_CHAT_UPDATE_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"
#include "SlackBlockKit.h"
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.update
Response:
{
    "channel": "C09RU2URYMS",
    "ts": "1764215452.433709",
    "text": "Update text.",
    "message":
    {
        "user": "U09S3D8R00Z",
        "type": "message",
        "ts": "",
        "bot_id": "B09RJ4A000K",
        "app_id": "A09RQFXSEKC",
        "text": "Update text.",
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
                "elements":
                [
                    {
                        "type": "rich_text_section",
                        "elements":
                        [
                            {
                                "type": "text",
                                "text": "Update text."
                            }
                        ]
                    }
                ]
            }
        ]
    },
    "ok": true
}
#endif

struct UpdateReply: public API::Reply
{
    std::string             channel;
    std::string             ts;
    std::string             text;
    OptMessage              message;
};
struct Update
{
    static constexpr char const* api = "https://slack.com/api/chat.update";
    static constexpr bool hasBody = true;
    using Reply = UpdateReply;

    std::string             channel;        // Channel containing the message to be updated. For direct messages, ensure that this value is a DM ID (starts with D) instead of a User ID (starts with either U or W).
    std::string             ts;             // Timestamp of the message to be updated.
    OptBool                 as_user;        // Pass true to update the message as the authed user. Bot users in this context are considered authed users.
    OptVecString            attachments;    // A JSON-based array of structured attachments, presented as a URL-encoded string.
                                            // TODO
                                            // Example: [{"pretext": "pre-hello", "text": "text-world"}]
    BlockKit::OptBlocks     blocks;         // A JSON-based array of structured blocks, presented as a URL-encoded string.
    OptString               markdown_text;  // Accepts message text formatted in markdown. This argument should not be used in conjunction with blocks or text. Limit this field to 12,000 characters.
    OptMetadata             metadata;       // JSON object with event_type and event_payload fields, presented as a URL-encoded string. If you don't include this field, the message's previous metadata will be retained. To remove previous metadata, include an empty object for this field. Metadata you post to Slack is accessible to any app or user who is a member of that workspace.
    OptBool                 link_names;     // Find and link channel names and usernames. Defaults to none. If you do not specify a value for this field, the original value set for the message will be overwritten with the default, none.
    OptBool                 parse;          // Change how messages are treated. Defaults to client, unlike chat.postMessage. Accepts either none or full. If you do not specify a value for this field, the original value set for the message will be overwritten with the default, client.
    OptString               text;           // How this field works and whether it is required depends on other fields you use in your API call. See below for more detail.
    OptBool                 reply_broadcast;// Broadcast an existing thread reply to make it visible to everyone in the channel or conversation.
    OptVecString            file_ids;       // Array of new file ids that will be sent with this message.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::UpdateReply, channel, ts, text, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Update, channel, ts, as_user, attachments, blocks, markdown_text, metadata, link_names, parse, text, reply_broadcast, file_ids);

#endif
