#ifndef THORSANVIL_SLACK_EVENT_PIN_ADDED_H
#define THORSANVIL_SLACK_EVENT_PIN_ADDED_H

// Documentation: https://docs.slack.dev/reference/events/reaction_added/

#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::Slack::Event
{
#if 0
https://docs.slack.dev/reference/events/pin_added
// Pin Added
// https://docs.slack.dev/reference/events/pin_removed
// Pin Removed
#endif

struct PinItem
{
    std::string                 type;                   // doc
    std::string                 channel;                // doc
    std::string                 ts;                     // doc
};
struct PinAdded
{
    // std::string                 type;                   // doc
    std::string                 user;
    std::string                 channel_id;
    PinItem                     item;
    bool                        has_pins;
    std::string                 event_ts;
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::ReactionAdded, pin_added);
    ThorsAnvil_TypeFieldName(type);
};
struct PinRemoved
{
    // std::string                 type;
    std::string                 user;
    std::string                 channel_id;
    PinItem                     item;
    bool                        has_pins;
    std::string                 event_ts;
    ThorsAnvil_VariantSerializerWithName(ThorsAnvil::Slack::Event::ReactionAdded, pin_removed);
    ThorsAnvil_TypeFieldName(type);
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::PinItem, type, channel, ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::PinAdded, user, channel_id, item, has_pins, event_ts);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::PinRemoved, user, channel_id, item, has_pins, event_ts);

#endif
