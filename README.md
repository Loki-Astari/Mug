# Mug Server

![Mug](img/Mug.jpg)

The Mug server is designed to be a lightweight server that will dynamically load configured C++ modules.

If the modules change (are rebuilt) Mug will auto-detect this update and load this new version automatically allowing a very simple build/test cycle, trying to be similar to the Python Jug concept.

Modules implement the "MugPlugin" interface which allows you to install REST based listeners on a ["NissaServer"](https://github.com/Loki-Astari/Nisse) that provide all the underlying functionality for async processing of HTTP(S) requests.

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
Valid values are [ <b>Error</b> | <b>Warning</b> | <b>Info</b> | <b>Debug</b> | <b>Track</b> | <b>Trace</b> | <b>All</b> | <b>0-9</b> ]<br>
Default Log level is: <b>0 (Info)</b><br>
<table>
<thead><tr><th>Name</th><th>Value</th></tr></thead>
<tbody>
<tr><td>Error</td><td>-2</td></tr> 
<tr><td>Warning</td><td>-1</td></tr> 
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

````JSON
{
    "controlPort":      <port>,
    "libraryCheckTime": <TimeCheck>,
    servers: [
       <Server Config: See Below>
    ]
}
````

* controlPort: Use this port to send commands to the server.
  - curl `https://host:&lt;port&gt;?command=stophard`  Immediately exit the server.
  - url `https://host:&lt;port&gt;?command=stopsoft`   Stop accepting requests. Exit when all current requests are complete.   
 
  Note: This port is usually not exposed outside the server and used for development and configuration. 
* libraryCheckTime: Time in milliseconds to check for changes to any plugins.
  - A value of 0 means no checks are done.
  - Recommended value 500
* servers: An array of "Server Configuration".

### Server Configuration:
A server configuration looks like this:

````json
{
    "port":         <port>,
    "certPath":     <path>,
    "actions":  [
         <Plugin To Load>
    ]
}
````
* port:  The port you want to connect a plugin to.
* certpath: Path in the local filesystem where the certificate and chain file are located.  
  Note: This value is optional.  
  - If not provided, a regular HTTP socket is created.  
  - But if provided, an HTTPS-encrypted socket is created.
* actions: An array of "Action Configuration".

### Action Configuration:

````json
{
     "pluginPath":    <path>,
     "configPath":    <config>
}
````
* pluginPath: Path in the local filesystem to a shared library for the platform that implements the Mug Interface.
* configPath: A string passed to `initPlugin()`. Recommend that this is a path to a configuration file for the plugin.

Each shared library specified in any "action" configuration is dynamically loaded, and the `spinUp()` method is called. Then `initPlugin(<handler>, <configPath>)` is called.

Note: If a shared library is specified in multiple "Action Configuration", then `spinUp()` is only called once, but `initPlugin()` is called each time.


## Implementing a module:

Implement the interface ThorsAnvil::ThorMug::MugPlugin (See ThorsMug/MugPlugin.h)  

````C++
    class MugPlugin
    {
        public:
            virtual ~MugPlugin() {}
            virtual void spinUp()                                                           {}
            virtual void spinDown()                                                         {}
            virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& config)   = 0;
            virtual void destPlugin(NisHttp::HTTPHandler& handler)                              = 0;
    };
````

The `spinUp()` and `spinDown()` methods get called when the module is first loaded/unloaded. The `initPlugin()` and `destPlugin()` are called for each configuration specified (See the configuration file).

## Exposing a module:

The following C code is used to expose a module from a shared library:

````C++
class SlackBot: public ThorsAnvil::ThorsMug::MugPlugin
{
     // Implementation of MugInterface
};
SlackBot            slackBot;

extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&slackBot);
}
````

## Implementing a handler

Details of implementing handlers can be found in [Nissa](https://github.com/Loki-Astari/Nisse). But a simple example:

````C++
class MyRestAction
{
    public:
        virtual void initPlugin(NisHttp::HTTPHandler& handler, std::string const& /*config*/) override
        {
             using ThorsAnvil::Nissa::HTTP::Method;
             using ThorsAnvil::Nissa::HTTP::Request;
             using ThorsAnvil::Nissa::HTTP::Reponse;

             handler.addPath(Method::GET, "/MyCheck/{Name}/{Age}",
                             [&]()Request& request, Response& response) {
                                  return handleMyCheck(request, response);
                             });
             handler.addPath(Method::POST, "/AddAddress/{Name}/{Age}",
                             [&]()Request& request, Response& response) {
                                  return handle addAddress(request, response);
                             });
        }
        virtual void destPlugin(NisHttp::HTTPHandler& handler) override
        {
             using ThorsAnvil::NissaHTTP::Method;

             handler.remPath(Method::GET, "/MyCheck/{Name}/{Age}");
             handler.remPath(Method::GET, "/AddAddress/{Name}/{Age}");
        }
    private:
        bool handleMyCheck(ThorsAnvil::NissaHTTP::Request& request, ThorsAnvil::NissaHTTP::Response& response)
        {
             using ThorsAnvil::Nissa::HTTP::Encoding;

             std::string const& name = request.variables()["Name"]; // From {Name} in the path
             std::string const& age  = request.variables()["Age"];  // From {Age}  in the path

             // If you know the exact size of the body you can pass that as a parameter to body.
             response.body(Encoding::Chunked) << "<html><head><title>" << name << "</title></head>"
                             << "<body>Got: " << name << " is " << age << " years old!</body></html>";

             // Default is a 200 OK
             //   Can set the status code to anything you like with response.setStatus()
             //   The result of body() is `std::ostream` that is connected to a socket.
             //   Once you start streaming to the body adding headers will result in an exception.
             return true;
             // If you return false, the server will see if other handlers have been mapped to this path.
        }
        bool addAddress(ThorsAnvil::NissaHTTP::Request& request, ThorsAnvil::NissaHTTP::Response& response)
        {
             std::string const& name = request.variables()["Name"];    // From {Name} in the path
             std::string const& age  = request.variables()["Age"];     // From {Age}  in the path
             std::string const& str1 = request.variables()["street1"]; // HTTP Header values available here.
             std::string const& str2 = request.variables()["street2"]; // Form attributes are available here.
             std::string const& pCode= request.variables()["postCode"];// query parameters are available here.

             // Update DB with address.
             // See ThorsSerializer for examples of sending JSON to a stream.
             // See ThorsMongo for examples of connecting to Mongo DB to store data.
             // No. Action returns a 200 OK.
             return true;
        }
};

MyRestAction            restAction;

extern "C" void* mugFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsMug::MugPlugin*>(&restAction);
}
````






