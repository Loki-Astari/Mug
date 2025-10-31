#ifndef THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H
#define THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H

#include "ChaliceConfig.h"
#include "NisseServer/NisseServer.h"
#include "NisseServer/PyntControl.h"
#include "NisseHTTP/HTTPHandler.h"

namespace NisServer = ThorsAnvil::Nisse::Server;
namespace NisHttp   = ThorsAnvil::Nisse::HTTP;

namespace ThorsAnvil::ThorsChalice
{

enum ChaliceServerMode {Headless, Active};

class ChaliceServer: public NisServer::NisseServer
{
    static constexpr std::size_t workerCount = 4;

    using Hanlders = std::vector<NisHttp::HTTPHandler>;

    // PyntControl create access point that can be used to cleanly shut down server.
    NisServer::PyntControl  control;
    // HTTPHandler
    Hanlders                servers;

    TASock::ServerInit getServerInit(std::optional<FS::path> certPath, int port);
    void handleRequest(NisHttp::Request& request, NisHttp::Response& response, FS::path const& contentDir);

    public:
        ChaliceServer(ChaliceConfig const& config, ChaliceServerMode mode);
};

}

#endif
