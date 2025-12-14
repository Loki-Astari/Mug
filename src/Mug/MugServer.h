#ifndef THORSANVIL_THORS_MUG_MUG_SERVER_H
#define THORSANVIL_THORS_MUG_MUG_SERVER_H

#include "ThorsMugConfig.h"
#include "MugConfig.h"
#include "DLLib.h"
#include "NisseServer/NisseServer.h"
#include "NisseServer/TimerAction.h"
#include "NisseHTTP/PyntHTTPControl.h"
#include "NisseHTTP/HTTPHandler.h"

#include <vector>
#include <optional>
#include <filesystem>


namespace ThorsAnvil::ThorsMug
{

using ThorsAnvil::Nisse::Server::NisseServer;
using ThorsAnvil::Nisse::Server::TimerAction;
using ThorsAnvil::Nisse::HTTP::HTTPHandler;
using ThorsAnvil::Nisse::HTTP::PyntHTTPControl;


enum MugServerMode {Headless, Active};

class MugServer: public NisseServer
{
    class LibraryChecker: public TimerAction
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

    using Hanlders = std::vector<HTTPHandler>;

    // PyntControl create access point that can be used to cleanly shut down server.
    PyntHTTPControl             control;
    Hanlders                    servers;
    DLLibMap                    libraries;
    LibraryChecker              libraryChecker;

    TASock::ServerInit getServerInit(std::optional<std::filesystem::path> certPath, int port);

    public:
        MugServer(MugConfig const& config, MugServerMode mode);

    private:
        void checkLibrary();
};

}

#endif
