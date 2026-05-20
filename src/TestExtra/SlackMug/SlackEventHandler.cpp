#include "SlackEventHandler.h"
#include "ThorsSlack/EventCallback.h"

SlackEventHandler::SlackEventHandler(std::string_view slackSecret, ThorsAnvil::Slack::SlackClient& client, std::map<std::string, int>& messageCount)
    : ThorsAnvil::Slack::SlackEventHandler(client, slackSecret, eventHandlerMap, slashCommandHandlerMap, actionHandlerMap, viewHandlerMap)
    , client(client)
    , messageCount(messageCount)
{
    eventHandlerMap[ThorsAnvil::Slack::Event::Message::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::Message> const& r)                      {handleCallbackMessageEvent(r);};

    eventHandlerMap[ThorsAnvil::Slack::Event::ReactionAdded::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionAdded> const& r)          {handleCallbackReactionAddedEvent(r);};
    eventHandlerMap[ThorsAnvil::Slack::Event::ReactionRemoved::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionRemoved> const& r)      {handleCallbackReactionRemovedEvent(r);};
    eventHandlerMap[ThorsAnvil::Slack::Event::PinAdded::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinAdded> const& r)                    {handleCallbackPinAddedEvent(r);};
    eventHandlerMap[ThorsAnvil::Slack::Event::PinRemoved::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinRemoved> const& r)                {handleCallbackPinRemovedEvent(r);};
    eventHandlerMap[ThorsAnvil::Slack::Event::StarAdded::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarAdded> const& r)                  {handleCallbackStarAddedEvent(r);};
    eventHandlerMap[ThorsAnvil::Slack::Event::StarAdded::typeName()]= [&](ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarRemoved> const& r)                {handleCallbackStarRemovedEvent(r);};
}

void SlackEventHandler::handleCallbackMessageEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::Message> const& r)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    ThorsAnvil::Slack::Event::Message const&    event    = r.event;

    ThorsLogInfo("SlackEventHandler", "handleCallbackMessageEvent", "Recievent Message Event");
    std::string const& userId = event.user;
    if ((userId != "") && (userId != client.getBotId())) {
        ++messageCount[userId];
        std::string channel = "@" + userId;
        std::string         text = "I see: " + event.text;

        client.sendMessage(ThorsAnvil::Slack::API::Chat::POSTMessage{.channel = channel, .text = text});
        if (event.text == "start") {
            sendWelcomeMessage(event.channel.value(), userId);
            return;
        }
        if (event.text.find("POP")) {
            client.sendMessage(ThorsAnvil::Slack::API::Chat::POSTMessage
                               {
                                    .channel = event.channel.value(),
                                    .text = "We saw a bad work!!!",
                                    .thread_ts = event.ts,
                               });
        }
    }
}

void SlackEventHandler::sendWelcomeMessage(std::string const& channel, std::string const& user)
{
    auto find = welcomeMessages.find(std::make_pair(channel, user));
    if (find == std::end(welcomeMessages)) {
        auto result = welcomeMessages.emplace(std::make_pair(channel, user), ThorsAnvil::Slack::WelcomeMessage{channel, user});
        find = result.first;
    }

    auto message = find->second.getMessage();
    client.sendMessage(message, [&find](ThorsAnvil::Slack::API::Chat::POSTMessage::Reply&& result){find->second.timestamp = std::stoi(result.ts);});
}

void SlackEventHandler::handleCallbackReactionAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionAdded> const& r)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    ThorsAnvil::Slack::Event::ReactionAdded const&    event    = r.event;

    ThorsLogInfo("SlackEventHandler", "handleCallbackReactionAddedEvent", "Recievent Reaction Add Event");

    std::string const& channel = event.item.channel;
    std::string const& user = event.user;

    auto find = welcomeMessages.find(std::make_pair(channel, user));
    if (find == std::end(welcomeMessages)) {
        return;
    }
    find->second.markCompleted();
    client.sendMessage(find->second.getMessage(), [&](auto&& resp){find->second.settime(std::stol(resp.ts));});
}

void SlackEventHandler::handleCallbackReactionRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::ReactionRemoved> const& /*r*/)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    //ThorsAnvil::Slack::Event::ReactionRemoved const&    event    = r.event;

    ThorsLogInfo("SlackPlugin", "handleCallbackReactionRemovedEvent", "Call to implemented method: ", "ReactionRemoved");
}

void SlackEventHandler::handleCallbackPinAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinAdded> const& /*r*/)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    //ThorsAnvil::Slack::Event::PinAdded const&    event    = r.event;

    ThorsLogInfo("SlackPlugin", "handleCallbackPinAddedEvent", "Call to implemented method: ", "PinAdded");
}

void SlackEventHandler::handleCallbackPinRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::PinRemoved> const& /*r*/)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    //ThorsAnvil::Slack::Event::PinRemoved const&    event    = r.event;

    ThorsLogInfo("SlackPlugin", "handleCallbackPinRemovedEvent", "Call to implemented method: ", "PinRemoved");
}

void SlackEventHandler::handleCallbackStarAddedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarAdded> const& /*r*/)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    //ThorsAnvil::Slack::Event::StarAdded const&    event    = r.event;

    ThorsLogInfo("SlackPlugin", "handleCallbackStarAddedEvent", "Call to implemented method: ", "StarAdded");
}

void SlackEventHandler::handleCallbackStarRemovedEvent(ThorsAnvil::Slack::EventRequest<ThorsAnvil::Slack::Event::StarRemoved> const& /*r*/)
{
    //ThorsAnvil::Nisse::HTTP::Request const&     request  = r.request;
    //ThorsAnvil::Nisse::HTTP::Response const&    response = r.response;
    //ThorsAnvil::Slack::Event::StarRemoved const&    event    = r.event;

    ThorsLogInfo("SlackPlugin", "handleCallbackStarRemovedEvent", "Call to implemented method: ", "StarRemoved");
}
