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
    public:
        SlackEventHandler(std::string_view slackSecret, ThorsAnvil::Slack::SlackClient& client, std::map<std::string, int>& messageCount);
    private:
        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::Message const& event) override;
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::ReactionAdded const& event) override;
        virtual void handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::ReactionRemoved const& event) override;
        virtual void handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::PinAdded const& event) override;
        virtual void handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::PinRemoved const& event) override;
        virtual void handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::StarAdded const& event) override;
        virtual void handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::StarRemoved const& event) override;

        void sendWelcomeMessage(std::string const& channel, std::string const& user);
};

#endif
