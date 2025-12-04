#ifndef THORSANVIL_THORS_MUG_MUG_SERVER_H
#define THORSANVIL_THORS_MUG_MUG_SERVER_H

#include "ThorsMugConfig.h"
#include "MugConfig.h"
#include "DLLib.h"
#include "NisseServer/NisseServer.h"
#include "NisseHTTP/PyntHTTPControl.h"
#include "NisseHTTP/HTTPHandler.h"

#include <cstddef>
#include <vector>
#include <optional>
#include <filesystem>
#include <utility>


namespace FS        = std::filesystem;
namespace NisServer = ThorsAnvil::Nisse::Server;
namespace NisHttp   = ThorsAnvil::Nisse::HTTP;

namespace ThorsAnvil::ThorsMug
{

enum MugServerMode {Headless, Active};

class MugServer: public NisServer::NisseServer
{
    class LibraryChecker: public NisServer::TimerAction
    {
        MugServer& server;
        public:
            LibraryChecker(MugServer& server)
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
    void handleRequestPath(NisHttp::Request& request, NisHttp::Response& response, std::filesystem::path const& contentDir);

    public:
        MugServer(MugConfig const& config, MugServerMode mode);

    private:
        void checkLibrary();
};

}

#endif
