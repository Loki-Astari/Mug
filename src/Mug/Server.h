#ifndef THORSANVIL_THORS_MUG_MUG_SERVER_H
#define THORSANVIL_THORS_MUG_MUG_SERVER_H

#include "ThorsMugConfig.h"
#include "MugConfig.h"
#include "DLLibMap.h"
#include "NisseServer/Server.h"
#include "NisseServer/TimerAction.h"
#include "NisseHTTP/PyntHTTPControl.h"
#include "NisseHTTP/HTTPHandler.h"

#include <atomic>
#include <vector>
#include <optional>
#include <filesystem>


extern "C"
{
    void mugSignalHandler(int);
}
namespace ThorsAnvil::ThorsMug
{

using ThorsAnvil::Nisse::Server::TimerAction;
using ThorsAnvil::Nisse::HTTP::HTTPHandler;
using ThorsAnvil::Nisse::HTTP::PyntHTTPControl;


enum ServerMode {Headless, Active};

class Server: public ThorsAnvil::Nisse::Server::Server
{
    class LibraryChecker: public TimerAction
    {
        Server& server;
        public:
            LibraryChecker(Server& server)
                : server(server)
            {}
            virtual void handleRequest(int) override
            {
                server.checkLibrary();
            }
    };

    class SignalChecker: public TimerAction
    {
        Server& server;
        public:
            SignalChecker(Server& server)
                : server(server)
            {}
            virtual void handleRequest(int) override
            {
                server.checkSignal();
            }
    };

    /*
     * Signal handler can set these flags.
     */
    friend void ::mugSignalHandler(int);
    static std::atomic<bool>    sighupReceived;
    static std::atomic<bool>    sigtermReceived;

    static constexpr std::size_t workerCount = 4;

    using Hanlders = std::vector<HTTPHandler>;

    // PyntControl create access point that can be used to cleanly shut down server.
    PyntHTTPControl             control;
    Hanlders                    servers;
    DLLibMap                    libraries;
    LibraryChecker              libraryChecker;
    SignalChecker               signalChecker;
    bool                        reloadFlag = false;

    TASock::ServerInit getServerInit(std::optional<std::filesystem::path> certPath, int port);

    public:
        Server(MugConfig const& config, ServerMode mode);

        bool reloadRequested() const    {return reloadFlag;}

    private:
        void checkLibrary();
        void checkSignal();

};

}

#if defined(THORSMUG_HEADER_ONLY) && THORSMUG_HEADER_ONLY == 1
#include "Server.source"
#endif

#endif
