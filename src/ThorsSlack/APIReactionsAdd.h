#ifndef THORSANVIL_SLACK_API_REACTIONS_ADD_H
#define THORSANVIL_SLACK_API_REACTIONS_ADD_H


#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Reactions
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/reactions.add/
Response:
{"ok":true}
#endif

struct Add
{
    static constexpr char const* api = "https://slack.com/api/reactions.add";
    static constexpr Method method = Method::POST;
    using Reply = API::Reply;

    std::string         channel;            // Channel where the message to add reaction to was posted.
    std::string         name;               // Reaction (emoji) name
    std::string         timestamp;          // Timestamp of the message to add reaction to.
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::Add, channel, name, timestamp);

#endif
