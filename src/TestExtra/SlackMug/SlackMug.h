#ifndef THORSANVIL_EXAMPLE_SLACKBOT_SLACKBOT_H
#define THORSANVIL_EXAMPLE_SLACKBOT_SLACKBOT_H

#include "SlackMugConfig.h"
#include "WelcomeMessage.h"
#include "ThorsSlack/EventCallbackReaction.h"
#include "ThorsSlack/EventCallbackMessage.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/SlackPlugin.h"
#include "ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <string>
#include <map>

class SlackMug: public ThorsAnvil::ThorsSlack::SlackPlugin
{
        using WelcomeMessage = ThorsAnvil::Slack::WelcomeMessage;

        ThorsAnvil::Slack::SlackClient          client;
        std::string                             botId;
        std::map<std::string, int>              messageCount;
        std::map<std::pair<std::string, std::string>, WelcomeMessage>   welcomeMessages;

        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& event) override;
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& event) override;
        virtual void handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionRemoved const& event) override;
        virtual void handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinAdded const& event) override;
        virtual void handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinRemoved const& event) override;
        virtual void handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarAdded const& event) override;
        virtual void handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarRemoved const& event) override;

        void handleCommand(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);

        void sendWelcomeMessage(std::string const& channel, std::string const& user);
    public:
        SlackMug();

        virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& config) override;
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override;

};

#endif
