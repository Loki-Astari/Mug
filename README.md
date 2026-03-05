# Mug Server

![Mug](img/Mug.jpg)

The Mug server is designed to be a lightweight server that will dynamically load configured C++ modules.

**Online documentation:**
- https://loki-astari.github.io/ThorsAnvil/Mug.html
- https://loki-astari.github.io/ThorsAnvil/ThorsMug.html
- https://loki-astari.github.io/ThorsAnvil/ThorsSlack.html

If the modules change (are rebuilt) Mug will auto-detect this update and load this new version automatically allowing a very simple build/test cycle, trying to be similar to the Python Jug concept.

Modules implement the `ThorsAnvil::ThorsMug::MugPlugin` interface which allows you to install REST based listeners on a ["NissaServer"](https://github.com/Loki-Astari/Nisse) that provide all the underlying functionality for async processing of HTTP(S) requests.

The "Mug" server is configured via a JSON based configuration file (see below)


## Command line arguments:

<table>
<thead><tr><th>Flag</th><th>Description</th></tr></thead>
<tbody>
<tr><td>--config=&lt;File&gt;</td>
<td>  
Default: The first of <code>./mug.cfg</code> <code>/etc/mug.cfg</code> <code>/opt/homebrew/etc/mug.cfg</code><br>
<br>  
The config file defines what modules are loaded and what ports they listen on (see below).  
</td></tr>
<tr><td>--logFile=&lt;file&gt;</td><td>Send log information to &lt;file&gt;.<br>Default is std::cerr.</td></tr>
<tr><td>--logSys</td><td>Send log information to syslog.<br>Default is std::cerr.</td></tr>
<tr><td>--logLevel=&lt;Level&gt;</td><td>Set the level of events that will be logged.<br>
Valid values are [ <b>Error</b> | <b>Warn</b> | <b>Info</b> | <b>Debug</b> | <b>Track</b> | <b>Trace</b> | <b>All</b> | <b>0-9</b> ]<br>
Default log level is whatever ThorsLogging defaults to (unless you override it via `--logLevel`).<br>
<table>
<thead><tr><th>Name</th><th>Value</th></tr></thead>
<tbody>
<tr><td>Error</td><td>-2</td></tr> 
<tr><td>Warn</td><td>-1</td></tr> 
<tr><td>Info</td><td>0</td></tr>
<tr><td>Debug</td><td>3</td></tr>
<tr><td>Track</td><td>5</td></tr>
<tr><td>Trace</td><td>7</td></tr>
<tr><td>All</td><td>9</td></tr>
</tbody></table>
See: <a href="https://github.com/Loki-Astari/ThorsLogging">ThorsLogging</a> for details.
</td></tr>
<tr><td>--silent</td><td>Don't print output to standard output.</td></tr>
<tr><td>--help</td><td>Show the help documentation.</td></tr>
</tbody>
</table>

## Config File:

### Mug Configuration:
The main "Mug" configuration looks like this.

```json
{
    "controlPort": 8079,
    "libraryCheckTime": 0,
    "servers": [
        { "port": 8080, "actions": [ /* ... */ ] }
    ]
}
```

* controlPort: Use this port to send commands to the server.
  - `curl "http://host:<port>/?command=stophard"` immediately exits the server.
 
  Note: This port is usually not exposed outside the server and used for development and configuration.
* libraryCheckTime: Time in milliseconds to check for changes to any plugins.
  - A value of 0 means no checks are done.
  - Recommended value 500
* servers: An array of "Server Configuration".
  - Note: When running the `mug` executable, this must not be empty.

### Server Configuration:
A server configuration looks like this:

```json
{
    "port": 8080,
    "certPath": "/etc/letsencrypt/live/example.com",
    "actions": [
        { "pluginPath": "/path/to/libMyPlugin.dylib", "config": "/page1" }
    ]
}
```
* port:  The port you want to connect a plugin to.
* certPath: Optional path to a directory containing `fullchain.pem` and `privkey.pem`.
  - If not provided, a regular HTTP socket is created.
  - If provided, an HTTPS-encrypted socket is created using those two files.
* actions: An array of "Action Configuration".

### Action Configuration:

```json
{
     "pluginPath": "/path/to/libMyPlugin.dylib",
     "config": { "any": "json" }
}
```
* pluginPath: Path in the local filesystem to a shared library for the platform that implements the Mug Interface.
* config: Any JSON value (string/object/array/etc). Mug passes the JSON text of this value into the plugin factory function (see below).

Each shared library specified in any "action" configuration is dynamically loaded, and for each configured action Mug calls the exported factory function `mugCreateInstance(configJson)` to get a `MugPlugin*`. Mug then calls `plugin->start(handler)` to register its routes.

Note: If a shared library is specified in multiple "Action Configuration", then the library is only loaded once, but `mugCreateInstance()` is called for each action.


## Implementing a module:

Implement the interface `ThorsAnvil::ThorsMug::MugPlugin` (see `src/ThorsMug/MugPlugin.h`).

```cpp
namespace ThorsAnvil::ThorsMug
{
    class MugPlugin
    {
        public:
            virtual ~MugPlugin() {}
            virtual void start(ThorsAnvil::Nisse::HTTP::HTTPHandler& handler) = 0;
            virtual void stop(ThorsAnvil::Nisse::HTTP::HTTPHandler& handler)  = 0;
    };
}
```

Mug will call `start()` after creating an instance and `stop()` when unloading/reloading.

If you want a very small plugin, you can implement `ThorsAnvil::ThorsMug::MugPluginSimple` and just return a list of routes (see `src/ThorsMug/MugPlugin.h`).

## Exposing a module:

Your shared library must export a C symbol named `mugCreateInstance`. Mug will look this up via `dlsym()` and call it once per configured action.

```cpp
#include "ThorsMug/MugPlugin.h"

class MyPlugin: public ThorsAnvil::ThorsMug::MugPluginSimple
{
    public:
        std::vector<ThorsAnvil::ThorsMug::Action> getAction() override
        {
            namespace NisHttp = ThorsAnvil::Nisse::HTTP;
            return {
                {
                    NisHttp::Method::GET,
                    "/health",
                    [](NisHttp::Request const&, NisHttp::Response& response)
                    {
                        response.setStatus(200);
                        return true;
                    }
                }
            };
        }
};

static MyPlugin plugin;

extern "C" ThorsAnvil::ThorsMug::MugPlugin* mugCreateInstance(char const* /*configJson*/)
{
    return &plugin;
}
```

Notes:
- `configJson` is the JSON text of the `config` value from the config file. For string configs this typically includes the quotes (e.g. `"/page1"`).
- Mug does not delete the returned pointer. Manage lifetime inside the plugin/library (static instance, internal singleton, etc).

## Implementing a handler

Details of implementing handlers can be found in [Nissa](https://github.com/Loki-Astari/Nisse). But a simple example:

```cpp
#include "ThorsMug/MugPlugin.h"

namespace NisHttp = ThorsAnvil::Nisse::HTTP;

class MyRestAction: public ThorsAnvil::ThorsMug::MugPluginSimple
{
    void handleMyCheck(NisHttp::Request const& request, NisHttp::Response& response)
    {
        std::string const& name = request.variables()["Name"]; // From {Name} in the path
        std::string const& age  = request.variables()["Age"];  // From {Age}  in the path

        response.body(NisHttp::Encoding::Chunked)
            << "<html><head><title>" << name << "</title></head>"
            << "<body>Got: " << name << " is " << age << " years old!</body></html>";
        response.setStatus(200);
    }

    public:
        std::vector<ThorsAnvil::ThorsMug::Action> getAction() override
        {
            return {
                {
                    NisHttp::Method::GET,
                    "/MyCheck/{Name}/{Age}",
                    [&](NisHttp::Request const& request, NisHttp::Response& response)
                    {
                        handleMyCheck(request, response);
                        return true;
                    }
                }
            };
        }
};

static MyRestAction plugin;

extern "C" ThorsAnvil::ThorsMug::MugPlugin* mugCreateInstance(char const* /*configJson*/)
{
    return &plugin;
}
```






