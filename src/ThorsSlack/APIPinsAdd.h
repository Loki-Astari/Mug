#ifndef THORSANVIL_SLACK_API_PINS_ADD_H
#define THORSANVIL_SLACK_API_PINS_ADD_H


#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Pins
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/pins.add/
Response:
{"ok":true}
#endif

struct Add
{
    static constexpr char const* api = "https://slack.com/api/pins.add";
    static constexpr Method method = Method::POST;
    using Reply = API::Reply;

    std::string         channel;            // Channel to pin the messsage to. You must also include a timestamp when pinning messages.
    OptString           timestamp;          // Timestamp of the message to pin. You must also include the channel.
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Pins::Add, channel, timestamp);

#endif
