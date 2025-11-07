#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <iostream>

void handle(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response)
{
    response.setStatus(501);
}

typedef void(*GenericFuncPtr)();

extern "C" GenericFuncPtr chaliceFunctionNotCorrectName()
{
    return reinterpret_cast<GenericFuncPtr>(&handle);
}
