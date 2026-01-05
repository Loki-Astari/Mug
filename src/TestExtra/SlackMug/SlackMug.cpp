#include "SlackMug.h"

#include "Environment.h"
#include "NisseHTTP/Util.h"


const Environment           environment("/Users/martinyork/Repo/ThorsMug/src/ThorsSlackMug/.slackenv");
std::unique_ptr<SlackMug>   slackMug;

extern "C" void* mugFunction(char const* config)
{
    slackMug.reset(new SlackMug(config));
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(slackMug.get());
}

namespace Ser       = ThorsAnvil::Serialize;
namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

SlackMug::SlackMug(char const*)
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

void SlackMug::initPlugin(NisHttp::HTTPHandler& handler)
{
    handler.addPath(NisHTTP::Method::POST, "/event",           [&](NisHTTP::Request& request, NisHTTP::Response& response){eventHandler.handleEvent(request, response);return true;}, [&](NisHTTP::Request& request){return eventHandler.validateRequest(request);});
    handler.addPath(NisHTTP::Method::POST, "/command/speak",   [&](NisHTTP::Request& request, NisHTTP::Response& response){handleCommand(request, response);return true;},            [&](NisHTTP::Request& request){return eventHandler.validateRequest(request);});
}
void SlackMug::destPlugin(NisHttp::HTTPHandler& handler)
{
    handler.remPath(NisHTTP::Method::POST, "/event");
    handler.remPath(NisHTTP::Method::POST, "/command/speak");
}
