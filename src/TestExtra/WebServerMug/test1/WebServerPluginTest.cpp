#include "gtest/gtest.h"

#include "ThorsWebServerConfig.h"
#include "WebServerPlugin.h"
#include "ThorsMug/MugServer.h"
#include "ThorsMug/MugConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "NisseHTTP/ClientRequest.h"

#if 0
#include "NisseHTTP/ClientStream.h"
#include "NisseHTTP/Util.h"
#include "ThorsSocket/Socket.h"
#include "ThorsSocket/SocketStream.h"

#include <sstream>
#endif

#include <latch>
#include <thread>

#define LOCAL_QUOTE1(X)     #X
#define LOCAL_QUOTE(X)      LOCAL_QUOTE1(X)
#define SLIB                "." LOCAL_QUOTE( SHARED_LIB_EXTENSION )

/*
 * Some locations were we build do not currently support std::jthread.
 * This is a simplified version just for testing purposes.
 */
//    std::jthread
class LocalJthread: public std::thread
{
    public:
        using std::thread::thread;
        ~LocalJthread()
        {
            join();
        }
};

TEST(MugServer, ServiceRunAddServerWithFile)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "ThorsWebBot" SLIB,
                            "configPath":  "test/data/pages"
                        }
                    ]
                }
            ]
        }
    )");
    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
    std::latch                          latch(1);

    auto work = [&]() {
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    // Touch the control point to shut down the server.
    latch.wait();
    ThorsAnvil::ThorsSocket::SocketStream       socket({"localhost", 8079});
    ThorsAnvil::Nisse::HTTP::HeaderResponse     headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

#if 0
TEST(MugServer, ServiceRunAddServerWithFileValidateWorks)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "type":     "File",
                            "rootDir":  "./test/data/pages",
                            "path":     "/files"
                        }
                    ]
                }
            ]
        }
    )");

    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
    std::latch                          latch(1);
    std::latch                          waitForExit(1);

    auto work = [&]() {
        server.run(
                [&latch](){latch.count_down();}
        );
        waitForExit.count_down();
    };

    LocalJthread     serverThread(work);
    latch.wait();

    ThorsAnvil::ThorsSocket::SocketStream socketData({"localhost", 8070});

    socketData << ThorsAnvil::ThorsSocket::HTTPSend(ThorsAnvil::ThorsSocket::SendType::GET, ThorsAnvil::ThorsSocket::SendVersion::HTTP1_1, "localhost", "/files/page1");

    ThorsAnvil::ThorsSocket::HTTPResponse   response;
    socketData >> response;

    ASSERT_EQ("Data for page 1\n", response.getBody());

    // Touch the control point to shut down the server.
    ThorsAnvil::ThorsSocket::SocketStream       socket({"localhost", 8079});
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
    request.flushRequest();
    waitForExit.wait();
}
#endif
