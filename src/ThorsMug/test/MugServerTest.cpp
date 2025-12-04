#include "gtest/gtest.h"
#include <string>
#include <thread>
#include "MugConfig.h"
#include "MugServer.h"

#include "NisseHTTP/ClientStream.h"
#include "NisseHTTP/ClientRequest.h"
#include "NisseHTTP/Util.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsSocket/Socket.h"
#include "ThorsSocket/SocketStream.h"

#include "HTTPSend.h"
#include "HTTPResponse.h"

#include <sstream>
#include <latch>

//using namespace std::chrono_literals;

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

TEST(MugServer, CreateHeadless)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    ThorsAnvil::ThorsMug::MugServerMode mode = ThorsAnvil::ThorsMug::Headless;
    ThorsAnvil::ThorsMug::MugServer     server(config, mode);
}
TEST(MugServer, CreateActive)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    ThorsAnvil::ThorsMug::MugServerMode mode = ThorsAnvil::ThorsMug::Active;
    ThorsAnvil::ThorsMug::MugServer     server(config, mode);
}
TEST(MugServer, ServiceRunManuallyStopped)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
    std::latch                          latch(1);

    auto work = [&]() {
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    latch.wait();
    server.stopHard();
}

TEST(MugServer, ServiceRunDefaultConfigHitControl)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
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
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

TEST(MugServer, ServiceRunModifiedControl)
{
    using ThorsAnvil::ThorsMug::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8078,
            "servers": []
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
    ThorsAnvil::ThorsSocket::SocketStream       socket({"localhost", 8078});
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

TEST(MugServer, ServiceRunAddServer)
{
    using ThorsAnvil::ThorsMug::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
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
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

TEST(MugServer, ServiceRunAddServerWithFile)
{
    using ThorsAnvil::ThorsMug::ActionType;
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
                            "path":     "page1"
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
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

TEST(MugServer, ServiceRunAddServerWithFileValidateWorks)
{
    using ThorsAnvil::ThorsMug::ActionType;
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
    // request.flushRequest();
    waitForExit.wait();
}

TEST(MugServer, CallALoadedLib)
{
    using ThorsAnvil::ThorsMug::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "type":     "Lib",
                            "rootDir":  "../L3/release/libL3.dylib",
                            "path":     "/page1"
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
    latch.wait();

    // Talk to server.
    ThorsAnvil::ThorsSocket::SocketStream socketData({"localhost", 8070});

    socketData << ThorsAnvil::ThorsSocket::HTTPSend(ThorsAnvil::ThorsSocket::SendType::GET, ThorsAnvil::ThorsSocket::SendVersion::HTTP1_1, "localhost", "/Plop/");

    ThorsAnvil::ThorsSocket::HTTPResponse   response;
    socketData >> response;

    ASSERT_EQ(305, response.getCode());

    // Touch the control point to shut down the server.
    ThorsAnvil::ThorsSocket::SocketStream       socket({"localhost", 8079});
    ThorsAnvil::Nisse::HTTP::HeaderResponse   headers;
    headers.add("host", "localhost");
    headers.add("content-length", "0");
    ThorsAnvil::Nisse::HTTP::ClientRequest  request(socket, "localhost:/?command=stophard");
    request.addHeaders(headers);
}

