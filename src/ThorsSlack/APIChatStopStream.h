#ifndef THORSANVIL_SLACK_API_CHAT_STOP_STREAM_H
#define THORSANVIL_SLACK_API_CHAT_STOP_STREAM_H


#include "ThorSerialize/Traits.h"
#include "ThorsSlackConfig.h"
#include "APIChat.h"
#include "SlackBlockKit.h"
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.stopStream
#if 0
// TODO. Needs payed account to test and validate.
Expected Response:
{
  "ok": true,
  "channel": "C123ABC456",
  "ts": "1503435956.000247",
  "message":
  {
    "text": "Here's the final streamed message content",
    "bot_id": "B123ABC456",
    "ts": "1503435956.000247",
    "type": "message",
    "subtype": "bot_message"
  }
}
#endif

struct StopStreamReply: public API::Reply
{
    std::string             channel;
    std::string             ts;
    OptMessage              message;
};
struct StopStream
{
    static constexpr char const* api = "https://slack.com/api/chat.stopStream";
    static constexpr bool hasBody = true;
    using Reply = StopStreamReply;

    std::string             channel;            // An encoded ID that represents a channel, private group, or DM
    std::time_t             ts;                 // The timestamp of the streaming message.
    OptString               markdown_text;      // Accepts message text formatted in markdown. Limit this field to 12,000 characters.
    BlockKit::OptBlocks     blocks;             // A list of blocks that will be rendered at the bottom of the finalized message.
    OptMetadata             metadata;           // JSON object with event_type and event_payload fields, presented as a URL-encoded string. Metadata you post to Slack is accessible to any app or user who is a member of that workspace.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::StopStreamReply, channel, ts, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::StopStream, channel, ts, markdown_text, blocks, metadata);

#endif
