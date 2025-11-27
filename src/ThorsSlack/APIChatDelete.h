#ifndef THORSANVIL_SLACK_API_CHAT_DELETE_H
#define THORSANVIL_SLACK_API_CHAT_DELETE_H


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
  "ts": "1401383885.000061"
}
#endif

struct DeleteReply: public API::Reply
{
    std::string             channel;
    std::string             ts;
};
struct Delete
{
    static constexpr char const* api = "https://slack.com/api/chat.delete";
    static constexpr bool hasBody = true;
    using Reply = DeleteReply;

    std::string             channel;        // Channel containing the message to be deleted.
    std::string             ts;             // Timestamp of the message to be deleted.
    OptBool                 as_user;        // (Legacy) Pass true to delete the message as the authed user with chat:write:user scope. Bot users in this context are considered authed users. See legacy as_user parameter below.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::DeleteReply, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Delete, channel, ts, as_user);

#endif
