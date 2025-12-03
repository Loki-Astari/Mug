#include "MugServer.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseServer/NisseServer.h"
#include <filesystem>
#include <ratio>
#include <string>

using namespace ThorsAnvil::ThorsMug;


TASock::ServerInit MugServer::getServerInit(std::optional<FS::path> certPath, int port)
{
    if (!certPath.has_value()) {
        return TASock::ServerInfo{port};
    }

    TASock::CertificateInfo     certificate{FS::canonical(FS::path(*certPath) /= "fullchain.pem"),
                                            FS::canonical(FS::path(*certPath) /= "privkey.pem")
                                           };
    TASock::SSLctx              ctx{TASock::SSLMethodType::Server, certificate};
    return TASock::SServerInfo{port, std::move(ctx)};
}

void MugServer::handleRequestPath(NisHttp::Request& request, NisHttp::Response& response, FS::path const& contentDir)
{
    ThorsLogDebug("MugServer", "handleRequestLib", "Handle file extract");
    // Get the path from the HTTP request object.
    // Remove the leading slash if it exists.
    std::string_view    path = request.variables()["FilePath"];
    ThorsLogDebug("MugServer", "handleRequestPath", "Input Path:   ", path);

    // Check that the path is valid
    // i.e. Some basic checks that the user is not trying to break into the filesystem.
    FS::path            requestPath = FS::path(path).lexically_normal();
    ThorsLogDebug("MugServer", "handleRequestPath", "Request Path: ", requestPath.string());
    if (requestPath.empty() || (*requestPath.begin()) == "..") {
        ThorsLogDebug("MugServer", "handleRequestPath", "400 Invalid Path");
        response.error(400, "Invalid Request Path");
        return;
    }

    // Add the path to the root contentDir and check if the file exists.
    // Note if the user picked a directory we look for index.html
    std::error_code ec;
    FS::path        filePath = FS::path{contentDir} /= requestPath;
    ThorsLogDebug("MugServer", "handleRequestPath", "File Path:    ", filePath.string());
    filePath = FS::canonical(filePath, ec);
    ThorsLogDebug("MugServer", "handleRequestPath", "Conical Path: ", filePath.string());
    if (!ec && FS::is_directory(filePath)) {
        filePath = FS::canonical(filePath /= "index.html", ec);
    }
    if (ec || !FS::is_regular_file(filePath)) {
        ThorsLogDebug("MugServer", "handleRequestPath", "404 No File Found: ", filePath);
        response.error(404, "No File Found At Path");
        return;
    }

    // Create an async File stream for the file we found.
    TASock::SocketStream    file{TASock::Socket{TASock::FileInfo{filePath.string(), TASock::FileMode::Read}, TASock::Blocking::No}};
    NisServer::AsyncStream  async(file, request.getContext(), NisServer::EventType::Read);

    // Stream the file to the output stream (this is an async operation).
    // So if the streaming blocks the thread will be released to do other work.
    // But return here to complete the task when either the file or the socket become available.
    response.body(NisHttp::Encoding::Chunked) << file.rdbuf();
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
            ThorsLogDebug("MugServer", "MugServer", "  Adding Action: ", action.path);
            switch (action.type)
            {
                case ActionType::File:
                {
                    std::string     path = FS::path(action.path + "/{FilePath}").lexically_normal();
                    ThorsLogDebug("MugServer", "MugServer", "    File Listener: ", path);
                    servers.back().addPath(NisHttp::Method::GET,
                                           path,
                                           [&, rootDir = action.rootDir](NisHttp::Request& request, NisHttp::Response& response)
                                           {handleRequestPath(request, response, rootDir);}
                                          );
                    break;
                }
                case ActionType::Lib:
                {
                    ThorsLogDebug("MugServer", "MugServer", "    Lib  Listener: ");
                    std::size_t libIndex = libraries.load(action.rootDir);
                    libraries.registerHandlers(libIndex, servers.back(), action.path);
                    break;
                }
            }
        }
        listen(getServerInit(server.certPath, server.port), servers.back());
    }
    ThorsLogDebug("MugServer", "MugServer", "  Adding Control Port: ", config.controlPort);
    listen(TASock::ServerInfo{config.controlPort}, control);
    //control.

    using namespace std::chrono_literals;
    std::chrono::milliseconds   libraryCheckTime(config.libraryCheckTime);
    if (config.libraryCheckTime == 0) {
        libraryCheckTime = 1s;
    }
    addTimer(libraryCheckTime, libraryChecker);
}

void MugServer::checkLibrary()
{
    libraries.checkAll();
}
