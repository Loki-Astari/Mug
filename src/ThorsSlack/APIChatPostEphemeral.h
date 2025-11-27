#ifndef THORSANVIL_SLACK_API_CHAT_POST_EPHEMERAL_H
#define THORSANVIL_SLACK_API_CHAT_POST_EPHEMERAL_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.postephemeral
Response:
{
    "ok":true,
    "message_ts":"1764217657.001000"
}
#endif

struct PostEphemeralReply: public API::Reply
{
    std::string             message_ts;
};

struct PostEphemeral
{
    static constexpr char const* api = "https://slack.com/api/chat.postEphemeral";
    static constexpr Method method = Method::POST;
    using Reply = PostEphemeralReply;

    std::string             channel;            // Channel, private group, or IM channel to send message to. Can be an encoded ID, or a name.
    std::string             user;               // id of the user who will receive the ephemeral message. The user should be in the channel specified by the channel argument.
    OptBool                 as_user;            // (Legacy) Pass true to post the message as the authed user. Defaults to true if the chat:write:bot scope is not included. Otherwise, defaults to false.
    OptVecString            attachments;        // A JSON-based array of structured attachments, presented as a URL-encoded string.
    BlockKit::OptBlocks     blocks;             // A JSON-based array of structured blocks, presented as a URL-encoded string.
    OptString               icon_emoji;         // Emoji to use as the icon for this message. Overrides icon_url.
    OptString               icon_url;           // URL to an image to use as the icon for this message.
    OptBool                 link_names;         // Find and link channel names and usernames.
    OptString               markdown_text;      // Accepts message text formatted in markdown. This argument should not be used in conjunction with blocks or text. Limit this field to 12,000 characters.
    OptString               parse;              // Change how messages are treated. Defaults to none. See below.
    OptString               text;               // How this field works and whether it is required depends on other fields you use in your API call. See below for more detail.
    OptString               thread_ts;          // Provide another message's ts value to post this message in a thread. Avoid using a reply's ts value; use its parent's value instead. Ephemeral messages in threads are only shown if there is already an active thread.
    OptString               username;           // Set your bot's user name.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::PostEphemeralReply, message_ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostEphemeral, channel, user, attachments, blocks, icon_emoji, icon_url, link_names, markdown_text, parse, text, thread_ts, username);

#endif
