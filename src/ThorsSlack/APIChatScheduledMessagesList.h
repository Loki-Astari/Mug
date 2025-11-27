#ifndef THORSANVIL_SLACK_API_CHAT_SCHEDULED_MESSAGE_LIST_H
#define THORSANVIL_SLACK_API_CHAT_SCHEDULED_MESSAGE_LIST_H


#include "ThorsSlackConfig.h"
#include "APIChat.h"

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.scheduledmessages.list
Response:
{
  "ok": true,
  "scheduled_messages":
  [
    {
      "id": "Q09UVPWRPP1",
      "channel_id": "C09RU2URYMS",
      "post_at": 1764139506,
      "date_created": 1764139446,
      "text": "A timed message"
    },
    {
      "id": "Q0A00QD9K0U",
      "channel_id": "C09RU2URYMS",
      "post_at": 1764139506,
      "date_created": 1764139446,
      "text": "A timed message"
    }
  ],
  "response_metadata":
  {
    "next_cursor": ""
  }
}
#endif

struct ScheduledMessagesItem
{
    std::string                         id;
    std::string                         channel_id;
    std::time_t                         post_at;
    std::time_t                         date_created;
    std::string                         text;
};

struct ScheduledMessagesListReply: public API::Reply
{
    std::vector<ScheduledMessagesItem>  scheduled_messages;
    Cursor                              next_cursor;
};
struct ScheduledMessagesList
{
    static constexpr char const* api = "https://slack.com/api/chat.scheduledMessages.list";
    static constexpr bool hasBody = true;
    using Reply = ScheduledMessagesListReply;

    OptString       channel;            // The channel of the scheduled messages
    OptString       cursor;             // For pagination purposes, this is the cursor value returned from a previous call to chat.scheduledmessages.list indicating where you want to start this call from.
    OptString       latest;             // A Unix timestamp of the latest value in the time range
    OptInt          limit;              // Maximum number of original entries to return.
    OptString       oldest;             // A Unix timestamp of the oldest value in the time range
    OptString       team_id;            // encoded team id to list channels in, required if org token is used
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::ScheduledMessagesItem, id, channel_id, post_at, date_created, text);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::ScheduledMessagesListReply, scheduled_messages, next_cursor);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::ScheduledMessagesList, channel, cursor, latest, limit, oldest, team_id);

#endif
