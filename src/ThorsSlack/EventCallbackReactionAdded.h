#ifndef THORSANVIL_SLACK_EVENT_REACTION_ADDED_H
#define THORSANVIL_SLACK_EVENT_REACTION_ADDED_H

// Documentation: https://docs.slack.dev/reference/events/reaction_added/

#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"
#include <string>

namespace ThorsAnvil::Slack::Event
{
#if 0
// Reaction Added
{
  "token": "4axIIPlMUVqU5NWGL7OQOfO9",
  "team_id": "T095XJHH589",
  "context_team_id": "T095XJHH589",
  "context_enterprise_id": null,
  "api_app_id": "A09RQFXSEKC",
  "event":
  {
    "type": "reaction_added",
    "user": "U095XJHJ1J5",
    "reaction": "blush",
    "item":
    {
      "type": "message",
      "channel": "D09RQG5F3NE",
      "ts": "1763961926.408339"
    },
    "item_user": "U09S3D8R00Z",
    "event_ts": "1763966107.000100"
  },
  "type": "event_callback",
  "event_id": "Ev09UR3H5STX",
  "event_time": 1763966107,
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
  "event_context": "4-eyJldCI6InJlYWN0aW9uX2FkZGVkIiwidGlkIjoiVDA5NVhKSEg1ODkiLCJhaWQiOiJBMDlSUUZYU0VLQyIsImNpZCI6IkQwOVJRRzVGM05FIn0"
}
#endif

struct Item
{
    std::string                 type;                   // doc
    std::string                 channel;                // doc
    std::string                 ts;                     // doc
};
struct ReactionAdded
{
    // std::string                 type;                   // doc
    std::string                 user;                   // doc
    std::string                 reaction;               // doc
    Item                        item;                   // doc
    std::string                 item_user;
    std::string                 event_ts;               // doc
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::ReactionAdded, reaction_added);
    ThorsAnvil_TypeFieldName(type);
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Item, type, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::ReactionAdded, user, reaction, item, item_user, event_ts);

#endif
