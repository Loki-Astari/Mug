#include "Server.h"

#include "NisseServer/Server.h"
#include <chrono>
#include <csignal>

using namespace ThorsAnvil::ThorsMug;
using Sigaction = struct sigaction;

std::atomic<bool> Server::sighupReceived{false};
std::atomic<bool> Server::sigtermReceived{false};

THORSMUG_HEADER_ONLY_INCLUDE
void mugSignalHandler(int sig)
{
    if (sig == SIGHUP) {
        Server::sighupReceived.store(true, std::memory_order_relaxed);
    }
    if (sig == SIGTERM) {
        Server::sigtermReceived.store(true, std::memory_order_relaxed);
    }
}


THORSMUG_HEADER_ONLY_INCLUDE
TASock::ServerInit Server::getServerInit(std::optional<std::filesystem::path> certPath, int port)
{
    // If there is not certificate simply use a normal port.
    if (!certPath.has_value()) {
        return TASock::ServerInfo{port};
    }

    // We have a certificate path.
    // Assume the "full certificate chain" and "private key" files are on the path provided.
    TASock::CertificateInfo     certificate{std::filesystem::canonical(std::filesystem::path(*certPath) /= "fullchain.pem"),
                                            std::filesystem::canonical(std::filesystem::path(*certPath) /= "privkey.pem")
                                           };
    TASock::SSLctx              ctx{TASock::SSLMethodType::Server, certificate};
    // Return an SSL configured port
    return TASock::SServerInfo{port, std::move(ctx)};
}

THORSMUG_HEADER_ONLY_INCLUDE
Server::Server(MugConfig const& config, ServerMode /*mode*/)
    : ThorsAnvil::Nisse::Server::Server(workerCount)
    , control(*this)
    , libraryChecker(*this)
    , signalChecker(*this)
{
    ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "Create Server");
    servers.reserve(config.servers.size());

    for (auto const& server: config.servers) {
        ThorsLogDebug("ThorsAnvil::ThorsMug::Server", "Server", "Adding Server: ", server.port);
        servers.emplace_back();
        for (auto const& action: server.actions) {
            ThorsLogDebug("ThorsAnvil::ThorsMug::Server", "Server", "  Adding Action: ", action.pluginPath, " Config: ", action.config.getString());

            libraries.load(servers.back(), action);
        }
        listen(getServerInit(server.certPath, server.port), servers.back());
    }
    ThorsLogDebug("ThorsAnvil::ThorsMug::Server", "Server", "  Adding Control Port: ", config.controlPort);
    listen(TASock::ServerInfo{config.controlPort}, control);

    using namespace std::chrono_literals;
    std::chrono::milliseconds   libraryCheckTime(config.libraryCheckTime);
    if (config.libraryCheckTime != 0) {
        addTimer(libraryCheckTime, libraryChecker);
    }

    Sigaction sa{};
    sa.sa_handler = mugSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    addTimer(500ms, signalChecker);
}

THORSMUG_HEADER_ONLY_INCLUDE
void Server::checkLibrary()
{
    libraries.checkAll();
}

THORSMUG_HEADER_ONLY_INCLUDE
void Server::checkSignal()
{
    if (sighupReceived.exchange(false, std::memory_order_relaxed)) {
        ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "checkSignal", "SIGHUP received, initiating reload");
        reloadFlag = true;
        stopSoft();
    }
    if (sigtermReceived.exchange(false, std::memory_order_relaxed)) {
        ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "checkSignal", "SIGTERM received, initiating shutdown");
        stopSoft();
    }
}
