#ifndef THORSANVIL_SLACK_API_CHAT_APPEND_STREAM_H
#define THORSANVIL_SLACK_API_CHAT_APPEND_STREAM_H


#include "ThorSerialize/Traits.h"
#include "ThorsSlackConfig.h"
#include "APIChat.h"
#include "SlackBlockKit.h"
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.appendStream
#if 0
#endif

struct AppendStreamReply: public API::Reply
{
    std::string             channel;
    std::time_t             ts;
};
struct AppendStream
{
    static constexpr char const* api = "https://slack.com/api/chat.appendStream";
    static constexpr bool hasBody = true;
    using Reply = AppendStreamReply;

    std::string             channel;            // An encoded ID that represents a channel, private group, or DM
    std::time_t             ts;                 // The timestamp of the streaming message.
    OptString               markdown_text;      // Accepts message text formatted in markdown. Limit this field to 12,000 characters.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::AppendStreamReply, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::AppendStream, channel, ts, markdown_text);

#endif
