#ifndef THORSANVIL_SLACK_EVENT_STAR_ADDED_H
#define THORSANVIL_SLACK_EVENT_STAR_ADDED_H

// Documentation: https://docs.slack.dev/reference/events/reaction_added/

#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::Slack::Event
{
#if 0
// https://docs.slack.dev/reference/events/star_added
// Pin Added
// https://docs.slack.dev/reference/events/star_removed
// Pin Removed
#endif

struct StarItem
{
    std::string                 type;                   // doc
    std::string                 channel;                // doc
    std::string                 ts;                     // doc
};
struct StarAdded
{
    // std::string                 type;                   // doc
    std::string                 user;
    StarItem                    item;
    std::string                 event_ts;
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::ReactionAdded, star_added);
    ThorsAnvil_TypeFieldName(type);
};
struct StarRemoved
{
    // std::string                 type;
    std::string                 user;
    StarItem                    item;
    std::string                 event_ts;
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::ReactionAdded, star_removed);
    ThorsAnvil_TypeFieldName(type);
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::StarItem, type, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::StarAdded, user, item, event_ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::StarRemoved, user, item, event_ts);

#endif
