#ifndef THORSANVIL_SLACK_API_CHAT_START_STREAM_H
#define THORSANVIL_SLACK_API_CHAT_START_STREAM_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.startstream
#if 0
// TODO. Needs payed account to test and validate.
// Expected Response:
{
  "ok": true,
  "channel": "C123ABC456",
  "ts": "1503435956.000247"
}
#endif

struct StartStreamReply: public API::Reply
{
    std::string             channel;
    std::string             ts;
};
struct StartStream
{
    static constexpr char const* api = "https://slack.com/api/chat.startStream";
    static constexpr bool hasBody = true;
    using Reply = API::Reply;

    std::string             channel;            // An encoded ID that represents a channel thread or DM.
    std::string             thread_ts;          // Provide another message's ts value to reply to. Streamed messages should always be replies to a user request.
    OptString               markdown_text;      // Accepts message text formatted in markdown. Limit this field to 12,000 characters.
    OptString               recipient_user_id;  // The encoded ID of the user to receive the streaming text. Required when streaming to channels.
    OptString               recipient_team_id;  // The encoded ID of the team the user receiving the streaming text belongs to. Required when streaming to channels.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::StartStreamReply, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::StartStream, channel, thread_ts, markdown_text, recipient_user_id, recipient_team_id);

#endif
