#ifndef THORSANVIL_SLACK_API_CHAT_GET_PERMALINK_H
#define THORSANVIL_SLACK_API_CHAT_GET_PERMALINK_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"
#include <string>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.delete
#if 0
// TODO. Needs payed account to test and validate.
// Expected Response:
{
  "ok": true,
  "channel": "C123ABC456",
  "permalink": "https://ghostbusters.slack.com/archives/C1H9RESGA/p135854651500008"
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
    static constexpr bool hasBody = false;
    using Reply = GetPermalinkReply;

    std::string             channel;        // The ID of the conversation or channel containing the message
    std::string             message_ts;     // A message's ts value, uniquely identifying it within a channel

    std::string             query() const   {return "channel=" + channel + "&message_ts=" + message_ts;}
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::GetPermalinkReply, channel, permalink);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::GetPermalink, channel, message_ts);

#endif
