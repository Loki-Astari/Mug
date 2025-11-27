#ifndef THORSANVIL_SLACK_API_CHAT_DELETE_SCHEDULE_MESSAGE_HH
#define THORSANVIL_SLACK_API_CHAT_DELETE_SCHEDULE_MESSAGE_HH


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.deletescheduledmessage
Response:
{"ok":true}
#endif

struct DeleteScheduledMessage
{
    static constexpr char const* api = "https://slack.com/api/chat.deleteScheduledMessage";
    static constexpr Method method = Method::POST;
    using Reply = API::Reply;

    std::string             channel;                // The channel the scheduled_message is posting to
    std::string             scheduled_message_id;   // scheduled_message_id returned from call to chat.scheduleMessage
    OptBool                 as_user;                // Pass true to delete the message as the authed user with chat:write:user scope. Bot users in this context are considered authed users.
                                                    // If unused or false, the message will be deleted with chat:write:bot scope.
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage, channel, scheduled_message_id, as_user);

#endif
