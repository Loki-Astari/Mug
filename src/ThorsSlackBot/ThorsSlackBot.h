#ifndef THORSANVIL_SLACK_THORSSLACKBOT_H
#define THORSANVIL_SLACK_THORSSLACKBOT_H

#include "../ThorsChalice/DLLib.h"
#include "Environment.h"
#include "ThorsSlack/EventCallbackReactionAdded.h"
#include "WelcomeMessage.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/Event.h"
#include <string>
#include <map>

class SlackBot: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
        struct VisitorEvent
        {
            SlackBot&           bot;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::EventURLVerification const& event)    {bot.handleURLVerificationEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::EventCallback const& event)           {bot.handleCallbackEvent(request, response, event);}
        };
        struct VisitorCallbackEvent
        {
            SlackBot&           bot;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::Message const& event)                 {bot.handleCallbackMessageEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::ReactionAdded const& event)           {bot.handleCallbackReactionAddedEvent(request, response, event);}
        };

        using WelcomeMessage = ThorsAnvil::Slack::WelcomeMessage;

        const Environment                       environment;
        ThorsAnvil::Slack::SlackClient          client;
        std::string                             botId;
        std::map<std::string, int>              messageCount;
        std::map<std::pair<std::string, std::string>, WelcomeMessage>   welcomeMessages;

        std::string getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response, bool& found);
        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        void handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event);
        void handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event);
        void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& event);
        void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& event);
        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
        void handleCommand(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);

        void sendWelcomeMessage(std::string const& channel, std::string const& user);
    public:
        SlackBot();

        virtual void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& name) override;

};

#endif
