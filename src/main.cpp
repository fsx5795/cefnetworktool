#include "app.h"
//#include "renderapp.h"

int main()
{
    CefMainArgs mainArgs(GetModuleHandle(nullptr));
    CefSettings settings;
    CefRefPtr<App> app(new App());
    //int exit_code = CefExecuteProcess(mainArgs, new RenderApp(), nullptr);
    int exit_code = CefExecuteProcess(mainArgs, app.get(), nullptr);
    if (exit_code >= 0)
        return exit_code;
    CefInitialize(mainArgs, settings, app.get(), nullptr);
    CefRunMessageLoop();
    CefShutdown();
    return 0;
}