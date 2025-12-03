#ifndef THORSANVIL_SLACK_EVENT_MESSAGE_H
#define THORSANVIL_SLACK_EVENT_MESSAGE_H


// Documentation: https://docs.slack.dev/reference/events/message.channels/

#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
#include <string>

namespace ThorsAnvil::Slack::Event
{
#if 0
// event_callback: message
{
  "token": "<XXX>",
  "team_id": "T095XJHH589",
  "context_team_id": "T095XJHH589",
  "context_enterprise_id": null,
  "api_app_id": "A09RQFXSEKC",
  "event":
  {
    "type": "message",
    "user": "U095XJHJ1J5",
    "ts": "1763965955.056429",
    "client_msg_id": "ab06af53-2535-4052-a8e8-e739b2766d9c",
    "text": "Plop",
    "team": "T095XJHH589",
    "blocks": [
      {
        "type": "rich_text",
        "block_id": "Yy7Sx",
        "elements": [
          {
            "type": "rich_text_section",
            "elements": [
              {
                "type": "text",
                "text": "Plop"
              }
            ]
          }
        ]
      }
    ],
    "channel": "<Your Slack Channel>",
    "event_ts": "1763965955.056429",
    "channel_type": "channel"
  },
  "type": "event_callback",
  "event_id": "Ev09UN4L40SX",
  "event_time": 1763965955,
  "authorizations": [
    {
      "enterprise_id": null,
      "team_id": "T095XJHH589",
      "user_id": "U09S3D8R00Z",
      "is_bot": true,
      "is_enterprise_install": false
    }
  ],
  "is_ext_shared_channel": false,
  "event_context": "4-eyJldCI6Im1lc3NhZ2UiLCJ0aWQiOiJUMDk1WEpISDU4OSIsImFpZCI6IkEwOVJRRlhTRUtDIiwiY2lkIjoiQzA5UlUyVVJZTVMifQ"
}
#endif
struct Message
{
    //std::string                 type;                           // doc
    std::string                 user;                           // doc
    std::string                 ts;                             // doc
    std::string                 client_msg_id;
    std::string                 text;                           // doc
    std::string                 team;
    BlockKit::OptBlocks         blocks;
    std::string                 channel;                        // doc
    std::string                 event_ts;                       // doc
    std::string                 channel_type;                   // doc
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::Message, message);
    ThorsAnvil_TypeFieldName(type);
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message, user, ts, client_msg_id, text, team, blocks, channel, event_ts, channel_type);

#endif
