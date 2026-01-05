#include "WebServerMug.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseServer/NisseServer.h"

#include <string_view>

using namespace ThorsAnvil::ThorsMug::WebServer;

namespace NisServer = ThorsAnvil::Nisse::Server;
namespace NisHttp   = ThorsAnvil::Nisse::HTTP;

std::unique_ptr<WebServerPlugin>     webServer;

extern "C" void* mugFunction(char const* config)
{
    webServer.reset(new WebServerPlugin(config));
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(webServer.get());
}

WebServerPlugin::WebServerPlugin(std::string const& config)
    : contentDir(config.substr(1, config.size() - 2))
{}

void WebServerPlugin::initPlugin(NisHttp::HTTPHandler& handler)
{
    handler.addPath(NisHttp::Method::GET,
                    "/files/{FilePath}",
                    [&](NisHttp::Request& request, NisHttp::Response& response)
                    {
                        handleRequestPath(request, response);
                        return true;
                    }
                   );
}

void WebServerPlugin::destPlugin(NisHttp::HTTPHandler& handler)
{
    handler.remPath(NisHttp::Method::GET, "/files/{FilePath}");
}

void WebServerPlugin::handleRequestPath(NisHttp::Request& request, NisHttp::Response& response)
{
    ThorsLogDebug("MugServer", "handleRequestLib", "Handle file extract");
    // Get the path from the HTTP request object.
    // Remove the leading slash if it exists.
    std::string_view    path = request.variables()["FilePath"];
    ThorsLogDebug("MugServer", "handleRequestPath", "Input Path:   ", path);

    // Check that the path is valid
    // i.e. Some basic checks that the user is not trying to break into the filesystem.
    std::filesystem::path            requestPath = std::filesystem::path(path).lexically_normal();
    ThorsLogDebug("MugServer", "handleRequestPath", "Request Path: ", requestPath.string());
    if (requestPath.empty() || (*requestPath.begin()) == "..") {
        ThorsLogDebug("MugServer", "handleRequestPath", "400 Invalid Path");
        response.error(400, "Invalid Request Path");
        return;
    }

    // Add the path to the root contentDir and check if the file exists.
    // Note if the user picked a directory we look for index.html
    std::error_code ec;
    std::filesystem::path        filePath = std::filesystem::path{contentDir} /= requestPath;
    ThorsLogDebug("MugServer", "handleRequestPath", "File Path:    ", filePath.string());
    filePath = std::filesystem::canonical(filePath, ec);
    ThorsLogDebug("MugServer", "handleRequestPath", "Conical Path: ", filePath.string());
    if (!ec && std::filesystem::is_directory(filePath)) {
        filePath = std::filesystem::canonical(filePath /= "index.html", ec);
    }
    if (ec || !std::filesystem::is_regular_file(filePath)) {
        ThorsLogDebug("MugServer", "handleRequestPath", "404 No File Found: ", filePath);
        response.error(404, "No File Found At Path");
        return;
    }

    // Create an nonblocking File stream for the file we found.
    TASock::SocketStream    file{TASock::Socket{TASock::FileInfo{filePath.string(), TASock::FileMode::Read}, TASock::Blocking::No}};
    if (!file) {
        // Could not open file so return a 404
        response.setStatus(404);
        return;
    }

    // Set the async callback functions.
    // If reading the file would cause the read to block this returns control of the thread to the server
    // to handle other work.
    NisServer::AsyncStream  async(file, request.getContext(), NisServer::EventType::Read);

    // Stream the file to the output stream (this is an async operation).
    // So if the streaming blocks the thread will be released to do other work.
    // But return here to complete the task when either the file or the socket become available.
    response.body(NisHttp::Encoding::Chunked) << file.rdbuf();
}
