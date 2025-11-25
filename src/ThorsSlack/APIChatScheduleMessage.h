#ifndef THORSANVIL_SLACK_API_CHAT_SCHEDULE_MESSAGE_HH
#define THORSANVIL_SLACK_API_CHAT_SCHEDULE_MESSAGE_HH


#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <optional>
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.scheduleMessage
#if 0
// Example Reply
#endif

using OptBool = std::optional<bool>;
using OptString = std::optional<std::string>;

struct Payload
{
    std::string                 id;
    std::string                 title;
};
struct Metadata
{
    std::string                 event_type;
    Payload                     event_payload;
};
using OptMetadata = std::optional<Metadata>;
struct ScheduleMessage
{
    static constexpr char const* api = "https://slack.com/api/chat.scheduleMessage";
    static constexpr bool hasBody = true;
    using Reply = int;

    std::string                 channel;                // Channel, private group, or DM channel to send message to. Can be an encoded ID, or a name. See below for more details.
    int                         post_at;                // Unix timestamp representing the future time the message should post to Slack.

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

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Payload, id, title);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Metadata, event_type, event_payload);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::ScheduleMessage, channel, post_at, as_user, attachments, blocks, link_names, markdown_text, parse, reply_broadcast, text, thread_ts, unfurl_links, unfurl_media, metadata);

#endif
