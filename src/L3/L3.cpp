#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <iostream>
#include <thread>
#include <chrono>

void handle(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    response.setStatus(305);
    auto sleepTime = request.variables()["sleep"];
    if (!sleepTime.empty()) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
    }
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
