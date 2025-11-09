#include "ThorsSlackBotConfig.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <iostream>
#include <thread>
#include <chrono>

std::string const SLACK_TOKEN = "<TOKEN>";
// REST:
//      https://slack.com/api/METHOD_FAMILY.method.
//      HTTPS, SSL, and TLS v1.2
//
//      GET querystring parameters,
//      POST parameters presented as application/x-www-form-urlencode


void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& /*response*/)
{
}

extern "C"
{
    typedef void(*GenericFuncPtr)();
}

extern "C" GenericFuncPtr chaliceFunction()
{
    GenericFuncPtr  result = (GenericFuncPtr)&handle;
    return result;
}
