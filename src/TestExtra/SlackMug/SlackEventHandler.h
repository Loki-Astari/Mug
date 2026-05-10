#ifndef THORSANVIL_EXAMPLE_SLACKBOT_SLACKEVENTHANDLER_H
#define THORSANVIL_EXAMPLE_SLACKBOT_SLACKEVENTHANDLER_H

#include "SlackMugConfig.h"
#include "ThorsSlack/EventCallback.h"
#include "WelcomeMessage.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/SlackEventHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

class SlackEventHandler: public ThorsAnvil::Slack::SlackEventHandler
{
    using WelcomeMessage = ThorsAnvil::Slack::WelcomeMessage;

    ThorsAnvil::Slack::SlackClient& client;
    std::map<std::string, int>&     messageCount;
    std::map<std::pair<std::string, std::string>, WelcomeMessage>   welcomeMessages;
    ThorsAnvil::Slack::EventHandlerMap     eventHandlerMap;
    public:
        SlackEventHandler(std::string_view slackSecret, ThorsAnvil::Slack::SlackClient& client, std::map<std::string, int>& messageCount);
    private:
        void handleCallbackMessageEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::Message> const& r);
        void handleCallbackReactionAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionAdded> const& r);
        void handleCallbackReactionRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionRemoved> const& r);
        void handleCallbackPinAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinAdded> const& r);
        void handleCallbackPinRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinRemoved> const& r);
        void handleCallbackStarAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarAdded> const& r);
        void handleCallbackStarRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarRemoved> const& r);

        void sendWelcomeMessage(std::string const& channel, std::string const& user);
};

#endif
