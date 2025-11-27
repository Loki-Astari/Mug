#ifndef THORSANVIL_SLACK_API_CHAT_SCHEDULE_MESSAGE_H
#define THORSANVIL_SLACK_API_CHAT_SCHEDULE_MESSAGE_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.scheduleMessage
Response:
{
    "scheduled_message_id": "Q0A0H8V2U2V",
    "channel": "C09RU2URYMS",
    "post_at": 1764215063,
    "message":
    {
        "user": "U09S3D8R00Z",
        "type": "message",
        "ts": "",
        "bot_id": "B09RJ4A000K",
        "app_id": "A09RQFXSEKC",
        "text": "I hope the tour went well, Mr. Wonka.",
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
                                "text": "I hope the tour went well, Mr. Wonka."
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

struct ScheduledMessageReply: public API::Reply
{
    std::string         scheduled_message_id;
    std::string         channel;
    std::time_t         post_at;
    OptMessage          message;
};


struct ScheduleMessage
{
    static constexpr char const* api = "https://slack.com/api/chat.scheduleMessage";
    static constexpr Method method = Method::POST;
    using Reply = ScheduledMessageReply;

    std::string                 channel;                // Channel, private group, or DM channel to send message to. Can be an encoded ID, or a name. See below for more details.
    std::time_t                 post_at;                // Unix timestamp representing the future time the message should post to Slack.

    OptBool                     as_user;                // Set to true to post the message as the authed user, instead of as a bot. Defaults to false. Cannot be used by new Slack apps. See chat.postMessage.
    OptString                   attachments;            // A JSON-based array of structured attachments, presented as a URL-encoded string.
    BlockKit::OptBlocks         blocks;                 // A JSON-based array of structured blocks, presented as a URL-encoded string.
    OptBool                     link_names;             // Find and link user groups. No longer supports linking individual users; use syntax shown in Mentioning Users instead.
    OptString                   markdown_text;          // Accepts message text formatted in markdown. This argument should not be used in conjunction with blocks or text. Limit this field to 12,000 characters.
    OptString                   parse;                  // Change how messages are treated. See chat.postMessage.
    OptBool                     reply_broadcast;        // Used in conjunction with thread_ts and indicates whether reply should be made visible to everyone in the channel or conversation. Defaults to false.
    OptString                   text;                   // How this field works and whether it is required depends on other fields you use in your API call. See below for more detail.
    OptString                   thread_ts;              // Provide another message's ts value to make this message a reply. Avoid using a reply's ts value; use its parent instead.
    OptBool                     unfurl_links;           // Pass true to enable unfurling of primarily text-based content.
    OptBool                     unfurl_media;           // Pass false to disable unfurling of media content.
    OptMetadata                 metadata;               // JSON object with event_type and event_payload fields, presented as a URL-encoded string. Metadata you post to Slack is accessible to any app or user who is a member of that workspace.

};


}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::ScheduledMessageReply, scheduled_message_id, channel, post_at, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::ScheduleMessage, channel, post_at, as_user, attachments, blocks, link_names, markdown_text, parse, reply_broadcast, text, thread_ts, unfurl_links, unfurl_media, metadata);

#endif
