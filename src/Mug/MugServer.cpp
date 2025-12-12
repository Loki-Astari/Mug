#include "MugServer.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseServer/NisseServer.h"
#include <fstream>
#include <ratio>
#include <string>
#include <string_view>
#include <chrono>

using namespace ThorsAnvil::ThorsMug;


TASock::ServerInit MugServer::getServerInit(std::optional<FS::path> certPath, int port)
{
    // If there is not certificate simply use a normal port.
    if (!certPath.has_value()) {
        return TASock::ServerInfo{port};
    }

    // We have a certificate path.
    // Assume the "full certificate chain" and "private key" files are on the path provided.
    TASock::CertificateInfo     certificate{FS::canonical(FS::path(*certPath) /= "fullchain.pem"),
                                            FS::canonical(FS::path(*certPath) /= "privkey.pem")
                                           };
    TASock::SSLctx              ctx{TASock::SSLMethodType::Server, certificate};
    // Return an SSL configured port
    return TASock::SServerInfo{port, std::move(ctx)};
}

MugServer::MugServer(MugConfig const& config, MugServerMode /*mode*/)
    : NisseServer(workerCount)
    , control(*this)
    , libraryChecker(*this)
{
    ThorsLogDebug("MugServer", "MugServer", "Create Server");
    servers.reserve(config.servers.size());

    for (auto const& server: config.servers) {
        ThorsLogDebug("MugServer", "MugServer", "Adding Server: ", server.port);
        servers.emplace_back();
        for (auto const& action: server.actions) {
            ThorsLogDebug("MugServer", "MugServer", "  Adding Action: ", action.pluginPath, " Config: ", action.configPath);

            libraries.load(servers.back(), action.pluginPath, action.configPath);
        }
        listen(getServerInit(server.certPath, server.port), servers.back());
    }
    ThorsLogDebug("MugServer", "MugServer", "  Adding Control Port: ", config.controlPort);
    listen(TASock::ServerInfo{config.controlPort}, control);

    using namespace std::chrono_literals;
    std::chrono::milliseconds   libraryCheckTime(config.libraryCheckTime);
    if (config.libraryCheckTime != 0) {
        addTimer(libraryCheckTime, libraryChecker);
    }
}

void MugServer::checkLibrary()
{
    libraries.checkAll();
}
