#include "SlackMug.h"

#include "WelcomeMessage.h"
#include "Environment.h"

#include "NisseHTTP/Util.h"

#include "ThorsSlack/APIAuth.h"
#include "ThorsSlack/APIChatMessage.h"
#include "ThorsSlack/APIChatSchedule.h"

#include <string>
#include <utility>
#include <cmath>

const Environment   environment("/Users/martinyork/Repo/ThorsMug/src/ThorsSlackMug/.slackenv");
SlackMug            slackMug;

extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&slackMug);
}

namespace Ser       = ThorsAnvil::Serialize;
namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

SlackMug::SlackMug()
    : SlackPlugin(environment.slackSecret)
    , client(environment.botToken, environment.userToken)
{
    client.sendMessage(ThorsAnvil::Slack::API::Auth::Test{}, [&](ThorsAnvil::Slack::API::Auth::Test::Reply&& result){botId = result.user_id;});
}

void SlackMug::initPlugin(NisHttp::HTTPHandler& handler, std::string const& /*name*/)
{
    handler.addPath(NisHTTP::Method::POST, "/event",           [&](NisHTTP::Request& request, NisHTTP::Response& response){handleEvent(request, response);return true;});
    handler.addPath(NisHTTP::Method::POST, "/command/speak",   [&](NisHTTP::Request& request, NisHTTP::Response& response){handleCommand(request, response);return true;});
}
void SlackMug::destPlugin(NisHttp::HTTPHandler& handler)
{
    handler.remPath(NisHTTP::Method::POST, "/event");
    handler.remPath(NisHTTP::Method::POST, "/command/speak");
}

void SlackMug::sendWelcomeMessage(std::string const& channel, std::string const& user)
{
    auto find = welcomeMessages.find(std::make_pair(channel, user));
    if (find == std::end(welcomeMessages)) {
        auto result = welcomeMessages.emplace(std::make_pair(channel, user), ThorsAnvil::Slack::WelcomeMessage{channel, user});
        find = result.first;
    }

    auto message = find->second.getMessage();
    client.sendMessage(message, [&find](ThorsAnvil::Slack::API::Chat::PostMessage::Reply&& result){find->second.timestamp = std::stoi(result.ts);});
}

void SlackMug::handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::Message const& event)
{
    ThorsLogInfo("SlackMug", "handleCallbackMessageEvent", "Recievent Message Event");
    std::string const& userId = event.user;
    if ((userId != "") && (userId != botId)) {
        ++messageCount[userId];
        std::string channel = "@" + userId;
        std::string         text = "I see: " + event.text;

        client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = text});
        if (event.text == "start") {
            sendWelcomeMessage(event.channel, userId);
            return;
        }
        if (event.text.find("POP")) {
            client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage
                               {
                                    .channel = event.channel,
                                    .text = "We saw a bad work!!!",
                                    .thread_ts = event.ts,
                               });
        }
    }
}
void SlackMug::handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::ReactionAdded const& event)
{
    ThorsLogInfo("SlackMug", "handleCallbackReactionAddedEvent", "Recievent Reaction Add Event");

    std::string const& channel = event.item.channel;
    std::string const& user = event.user;

    auto find = welcomeMessages.find(std::make_pair(channel, user));
    if (find == std::end(welcomeMessages)) {
        return;
    }
    find->second.markCompleted();
    client.sendMessage(find->second.getMessage(), [&](auto&& resp){find->second.settime(std::stol(resp.ts));});
}

void SlackMug::handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::ReactionRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackReactionRemovedEvent", "Call to implemented method: ", "ReactionRemoved");
}

void SlackMug::handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::PinAdded const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackPinAddedEvent", "Call to implemented method: ", "PinAdded");
}

void SlackMug::handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::PinRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackPinRemovedEvent", "Call to implemented method: ", "PinRemoved");
}

void SlackMug::handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::StarAdded const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackStarAddedEvent", "Call to implemented method: ", "StarAdded");
}

void SlackMug::handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/,ThorsAnvil::Slack::Event::StarRemoved const& /*event*/)
{
    ThorsLogInfo("SlackPlugin", "handleCallbackStarRemovedEvent", "Call to implemented method: ", "StarRemoved");
}

void SlackMug::handleCommand(NisHTTP::Request& request, NisHTTP::Response& response)
{
    ThorsLogDebug("SlackMug", "handleCommand", "Recievent Command");
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = "I have seen " + std::to_string(messageCount[userId])});
    response.setStatus(200);
}
