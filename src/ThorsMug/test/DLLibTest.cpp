#include "gtest/gtest.h"

#include "DLLib.h"
#include "NisseHTTP/HTTPHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseHTTP/URL.h"
#include "NisseHTTP/Util.h"
#include <filesystem>
#include <sstream>
#include <thread>

namespace FS = std::filesystem;

#define QUOTE1(X)   #X
#define QUOTE(X)    QUOTE1(X)
#define SLIB        "." QUOTE( SHARED_LIB_EXTENSION )


TEST(DLLibTest, LoadL3)
{
    std::error_code ec;
    ThorsAnvil::ThorsMug::DLLib   dlLib1(FS::canonical(FS::path("../L3/release/libL3" SLIB), ec));
}
TEST(DLLibTest, LoadFailEmpty)
{
    std::error_code ec;
    auto action = [&](){ThorsAnvil::ThorsMug::DLLib   dlLib1(FS::canonical(FS::path("../../build/lib/libBadLibName" SLIB), ec));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}
TEST(DLLibTest, LoadFailWithPath)
{
    auto action = [&](){ThorsAnvil::ThorsMug::DLLib   dlLib1(FS::path("../../build/lib/libBadLibName" SLIB));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}
TEST(DLLibTest, LoadOKFunctionBad)
{
    std::error_code ec;
    auto action = [&](){ThorsAnvil::ThorsMug::DLLib   dlLib1(FS::canonical(FS::path("../L5/release/libL5" SLIB), ec));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}

TEST(DLLibTest, LoadL3Call)
{
    std::error_code                     ec;
    ThorsAnvil::ThorsMug::DLLib         dlLib1(FS::canonical(FS::path("../L3/release/libL3" SLIB), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    ThorsAnvil::Nisse::HTTP::HTTPHandler    handler;
    dlLib1.init(handler, "L3");
    handler.processRequest(request, response);
    EXPECT_EQ(305, response.getCode().code);
}
TEST(DLLibTest, LoadL4Call)
{
    std::error_code                     ec;
    ThorsAnvil::ThorsMug::DLLib         dlLib1(FS::canonical(FS::path("../L4/release/libL4" SLIB), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    ThorsAnvil::Nisse::HTTP::HTTPHandler    handler;
    dlLib1.init(handler, "L4");
    handler.processRequest(request, response);
    EXPECT_EQ(404, response.getCode().code);
}

TEST(DLLibTest, CallCheck)
{
    std::error_code                     ec;
    ThorsAnvil::ThorsMug::DLLib         dlLib1(FS::canonical(FS::path("../L4/release/libL4" SLIB), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    ThorsAnvil::Nisse::HTTP::HTTPHandler    handler;
    dlLib1.init(handler, "L4");
    handler.processRequest(request, response);
    EXPECT_EQ(404, response.getCode().code);
    EXPECT_EQ(false, dlLib1.check());
    handler.processRequest(request, response);
    EXPECT_EQ(404, response.getCode().code);
}

TEST(DLLibTest, CheckOnEmpty)
{
    ThorsAnvil::ThorsMug::DLLib         dlLib(FS::canonical(FS::path("../L3/release/libL3" SLIB)));
    EXPECT_EQ(false, dlLib.check());
}

