#include "SlackEventHandler.h"
#include "ThorsSlack/EventCallback.h"

SlackEventHandler::SlackEventHandler(std::string_view slackSecret, ThorsAnvil::Slack::SlackClient& client, std::map<std::string, int>& messageCount)
    : ThorsAnvil::Slack::SlackEventHandler(slackSecret)
    , client(client)
    , messageCount(messageCount)
{}

void SlackEventHandler::handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::Message const& event)
{
    ThorsLogInfo("SlackEventHandler", "handleCallbackMessageEvent", "Recievent Message Event");
    std::string const& userId = event.user;
    if ((userId != "") && (userId != client.getBotId())) {
        ++messageCount[userId];
        std::string channel = "@" + userId;
        std::string         text = "I see: " + event.text;

        client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = text});
        if (event.text == "start") {
            sendWelcomeMessage(event.channel.value(), userId);
            return;
        }
        if (event.text.find("POP")) {
            client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage
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
    client.sendMessage(message, [&find](ThorsAnvil::Slack::API::Chat::PostMessage::Reply&& result){find->second.timestamp = std::stoi(result.ts);});
}

void SlackEventHandler::handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::ReactionAdded const& event)
{
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

void SlackEventHandler::handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::ReactionRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackReactionRemovedEvent", "Call to implemented method: ", "ReactionRemoved");
}

void SlackEventHandler::handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::PinAdded const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackPinAddedEvent", "Call to implemented method: ", "PinAdded");
}

void SlackEventHandler::handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::PinRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackPinRemovedEvent", "Call to implemented method: ", "PinRemoved");
}

void SlackEventHandler::handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::StarAdded const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackStarAddedEvent", "Call to implemented method: ", "StarAdded");
}

void SlackEventHandler::handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request const& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/, ThorsAnvil::Slack::Event::EventCallback const&, ThorsAnvil::Slack::Event::StarRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackStarRemovedEvent", "Call to implemented method: ", "StarRemoved");
}
