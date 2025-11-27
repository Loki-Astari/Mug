#ifndef THORSANVIL_SLACK_API_CHAT_ME_MESSAGE_H
#define THORSANVIL_SLACK_API_CHAT_ME_MESSAGE_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.memessage
Response:
{"ok":true,"channel":"C09RU2URYMS","ts":"1764218489.801069"}
#endif

struct MeMessageReply: public API::Reply
{
    std::string         channel;
    std::string         ts;
};

struct MeMessage
{
    static constexpr char const* api = "https://slack.com/api/chat.meMessage";
    static constexpr Method method = Method::POST;
    using Reply = MeMessageReply;

    std::string         channel;        // Channel to send message to. Can be a public channel, private group or IM channel. Can be an encoded ID, or a name.
    std::string         text;           // Text of the message to send.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::MeMessageReply, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::MeMessage, channel, text);

#endif
