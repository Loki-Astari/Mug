#include "gtest/gtest.h"
#include <thread>
#include "ChaliceConfig.h"
#include "ChaliceServer.h"

#include "ThorSerialize/JsonThor.h"
#include "ThorsSocket/Socket.h"

#include <sstream>

TEST(ChaliceServer, CreateHeadless)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Headless;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);
}
TEST(ChaliceServer, CreateActive)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Active;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);
}
TEST(ChaliceServer, ServiceRunManuallyStopped)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);
    server.stop();
}

TEST(ChaliceServer, ServiceRunDefaultConfigHitControl)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);

    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunModifiedControl)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8078,    // Change the control port
            "servers": []
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (configStream >> ThorsAnvil::Serialize::jsonImporter(config)) {
        ASSERT_TRUE(false);
    }


    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8078});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunAddServer)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,    // Change the control port
            "servers": [
                {
                    "port":     8080,
                    "rootDir":  "./test/data/pages",
                    "actions": [
                    ],
                }
            ]
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (configStream >> ThorsAnvil::Serialize::jsonImporter(config)) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunAddServerWithFile)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,    // Change the control port
            "servers": [
                {
                    "port":     8080,
                    "rootDir":  "./test/data/pages",
                    "actions": [
                        {
                            "path":     "page1",
                            "type":     "File",
                            "value":    "XXX"
                        }
                    ],
                }
            ]
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (configStream >> ThorsAnvil::Serialize::jsonImporter(config)) {
        ASSERT_TRUE(false);
    }


    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

