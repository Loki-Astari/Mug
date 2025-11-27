#ifndef THORSANVIL_SLACK_API_CHAT_APPEND_STREAM_H
#define THORSANVIL_SLACK_API_CHAT_APPEND_STREAM_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.appendStream
TODO. Needs payed account to test and validate.
Expected-Response:
{
  "ok": true,
  "channel": "C123ABC456",
  "ts": "1503435956.000247"
}
#endif

struct AppendStreamReply: public API::Reply
{
    std::string             channel;
    std::string             ts;
};
struct AppendStream
{
    static constexpr char const* api = "https://slack.com/api/chat.appendStream";
    static constexpr Method method = Method::POST;
    using Reply = AppendStreamReply;

    std::string             channel;            // An encoded ID that represents a channel, private group, or DM
    std::time_t             ts;                 // The timestamp of the streaming message.
    OptString               markdown_text;      // Accepts message text formatted in markdown. Limit this field to 12,000 characters.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::AppendStreamReply, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::AppendStream, channel, ts, markdown_text);

#endif
