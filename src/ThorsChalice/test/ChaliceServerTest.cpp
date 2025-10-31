#include "gtest/gtest.h"
#include <thread>
#include "ChaliceConfig.h"
#include "ChaliceServer.h"

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
TEST(ChaliceServer, ServiceRun)
{
    using ThorsAnvil::ThorsChalice::ActionType;

    ThorsAnvil::ThorsChalice::Action            action{"page", ActionType::File, "page"};
    ThorsAnvil::ThorsChalice::PortConfig        port{8080, {}, "./test/data/pages", {action}};
    ThorsAnvil::ThorsChalice::ChaliceConfig     config{8079, {port}};
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Headless;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(250ms);
    server.stop();
}

