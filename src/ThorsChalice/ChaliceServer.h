#ifndef THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H
#define THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H

#include "ChaliceConfig.h"
#include "DLLib.h"
#include "NisseServer/NisseServer.h"
#include "NisseHTTP/PyntHTTPControl.h"
#include "NisseHTTP/HTTPHandler.h"
#include <filesystem>


namespace NisServer = ThorsAnvil::Nisse::Server;
namespace NisHttp   = ThorsAnvil::Nisse::HTTP;

namespace ThorsAnvil::ThorsChalice
{

enum ChaliceServerMode {Headless, Active};

class ChaliceServer: public NisServer::NisseServer
{
    class LibraryChecker: public NisServer::TimerAction
    {
        ChaliceServer& server;
        public:
            LibraryChecker(ChaliceServer& server)
                : server(server)
            {}
            virtual void handleRequest(int) override
            {
                server.checkLibrary();
            }
    };

    static constexpr std::size_t workerCount = 4;

    using Hanlders = std::vector<NisHttp::HTTPHandler>;

    // PyntControl create access point that can be used to cleanly shut down server.
    NisHttp::PyntHTTPControl    control;
    // HTTPHandler
    DLLibMap                    libraries;
    LibraryChecker              libraryChecker;
    Hanlders                    servers;

    TASock::ServerInit getServerInit(std::optional<FS::path> certPath, int port);
    void handleRequestPath(NisHttp::Request& request, NisHttp::Response& response, FS::path const& contentDir);

    public:
        ChaliceServer(ChaliceConfig const& config, ChaliceServerMode mode);

    private:
        void checkLibrary();
};

}

#endif
