#include "SlackMug.h"

#include "Environment.h"
#include "NisseHTTP/Util.h"


const Environment   environment("/Users/martinyork/Repo/ThorsMug/src/ThorsSlackMug/.slackenv");
SlackMug            slackMug;

extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&slackMug);
}

namespace Ser       = ThorsAnvil::Serialize;
namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

SlackMug::SlackMug()
    // : SlackPlugin(environment.slackSecret)
    : client(environment.botToken, environment.userToken)
    , eventHandler(environment.slackSecret, client, messageCount)
{}

void SlackMug::handleCommand(NisHTTP::Request& request, NisHTTP::Response& response)
{
    ThorsLogDebug("SlackMug", "handleCommand", "Recievent Command");
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = "I have seen " + std::to_string(messageCount[userId])});
    response.setStatus(200);
}

void SlackMug::initPlugin(NisHttp::HTTPHandler& handler, std::string const& /*name*/)
{
    handler.addPath(NisHTTP::Method::POST, "/event",           [&](NisHTTP::Request& request, NisHTTP::Response& response){eventHandler.handleEvent(request, response);return true;});
    handler.addPath(NisHTTP::Method::POST, "/command/speak",   [&](NisHTTP::Request& request, NisHTTP::Response& response){handleCommand(request, response);return true;});
}
void SlackMug::destPlugin(NisHttp::HTTPHandler& handler)
{
    handler.remPath(NisHTTP::Method::POST, "/event");
    handler.remPath(NisHTTP::Method::POST, "/command/speak");
}
