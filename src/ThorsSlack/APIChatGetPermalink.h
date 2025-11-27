#ifndef THORSANVIL_SLACK_API_CHAT_GET_PERMALINK_H
#define THORSANVIL_SLACK_API_CHAT_GET_PERMALINK_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.getpermalink
Response:
{
    "channel": "C09RU2URYMS",
    "permalink": "https://thorsanvilworkspace.slack.com/archives/C09RU2URYMS/p1764214542336409",
    "ok": true
}
#endif

struct GetPermalinkReply: public API::Reply
{
    std::string             channel;
    std::string             permalink;
};
struct GetPermalink
{
    static constexpr char const* api = "https://slack.com/api/chat.getPermalink";
    static constexpr Method method = Method::GET;
    using Reply = GetPermalinkReply;

    std::string             channel;        // The ID of the conversation or channel containing the message
    std::string             message_ts;     // A message's ts value, uniquely identifying it within a channel

    std::string query() const {return buildQuery(std::tie("channel", channel), std::tie("message_ts", message_ts));}
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::GetPermalinkReply, channel, permalink);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::GetPermalink, channel, message_ts);

#endif
