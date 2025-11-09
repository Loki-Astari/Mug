#include "SlackStream.h"

#include <utility>

using namespace ThorsAnvil::Slack;

SlackStream::SlackStream(std::string authToken)
    : HTTPSStream("slack.com")
    , authToken(std::move(authToken))
{}

namespace ThorsAnvil::Slack
{

std::ostream& operator<<(SlackStream& stream, ThorsAnvil::Nisse::HTTP::HTTPRequest const& request)
{
    ThorsAnvil::Nisse::HTTP::HTTPSStream& parentStream = stream;
    return parentStream << request
                        << "Authorization: Bearer " << stream.authToken << "\r\n"
                        << "Content-Type: application/json; charset=utf-8\r\n";
}

}
