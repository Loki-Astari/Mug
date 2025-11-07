#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <iostream>

void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
{
    response.setStatus(404);
}

typedef void(*GenericFuncPtr)();

extern "C" GenericFuncPtr chaliceFunction()
{
    return reinterpret_cast<GenericFuncPtr>(&handle);
}
