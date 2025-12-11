# Mug Server

![Mug](img/Mug.jpg)

The Mug server is designed to be a light weight server that will dynamically load configured C++ modules.
If the modules change (are rebuilt) Mug will auto-detect this change and load this new version automatically allowing a very simple build/test cycle.



# Implementing a module

Implement the int MugPlugin interface.

```` C++
   class MugPlugin
   {
      public:
          virtual ~MugPlugin() {}
          virtual void spinUp(NisHttp::HTTPHandler& handler, std::string const& name) = 0;
          virtual void spinDown() = 0;
   };
````

The `spinUp()` method gets called when the module is first loaded and `spindDown()` will be called when your module is unloaded. 





