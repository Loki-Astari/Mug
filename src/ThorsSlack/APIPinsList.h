#ifndef THORSANVIL_SLACK_API_PINS_LIST_H
#define THORSANVIL_SLACK_API_PINS_LIST_H


#include "ThorSerialize/Traits.h"
#include "ThorsSlackConfig.h"
#include "API.h"
#include <ctime>
#include <string>

namespace ThorsAnvil::Slack::API::Pins
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/pins.list/
Response:
{
  "ok": true,
  "items": [
    {
      "type": "message",
      "created": 1764228505,
      "created_by": "U09S3D8R00Z",
      "channel": "C09RU2URYMS",
      "message":
      {
        "user": "U09S3D8R00Z",
        "type": "message",
        "ts": "1764228505.264229",
        "bot_id": "B09RJ4A000K",
        "app_id": "A09RQFXSEKC",
        "text": "I hope the tour went well, Mr. Wonka.",
        "team": "T095XJHH589",
        "bot_profile":
        {
          "id": "B09RJ4A000K",
          "app_id": "A09RQFXSEKC",
          "user_id": "U09S3D8R00Z",
          "name": "ThorsSlackBotOne",
          "icons":
          {
            "image_36": "https://a.slack-edge.com/80588/img/plugins/app/bot_36.png",
            "image_48": "https://a.slack-edge.com/80588/img/plugins/app/bot_48.png",
            "image_72": "https://a.slack-edge.com/80588/img/plugins/app/service_72.png"
          },
          "deleted": false,
          "updated": 1762644664,
          "team_id": "T095XJHH589"
        },
        "blocks": [
          {
            "type": "rich_text",
            "block_id": "LNWg",
            "elements": [
              {
                "type": "rich_text_section",
                "elements": [
                  {
                    "type": "text",
                    "text": "I hope the tour went well, Mr. Wonka."
                  }
                ]
              }
            ]
          }
        ],
        "pinned_to": [
          "C09RU2URYMS"
        ],
        "permalink": "https://thorsanvilworkspace.slack.com/archives/C09RU2URYMS/p1764228505264229"
      }
    },
  ]
}
#endif

struct ListMessage
{
    std::string         type;
    std::time_t         created;
    std::string         created_by;
    std::string         channel;
    API::Message        message;
};
using VecListMessage = std::vector<ListMessage>;

struct ListReply: public API::Reply
{
    VecListMessage      items;
};
struct List
{
    static constexpr char const* api = "https://slack.com/api/pins.list";
    static constexpr Method method = Method::GET;
    using Reply = ListReply;

    std::string         channel;        // Channel to get pinned items for.

    std::string query() const {return buildQuery(std::tie("channel", channel));}
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Pins::ListMessage, type, created, created_by, channel, message);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Pins::ListReply, items);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Pins::List, channel);

#endif
