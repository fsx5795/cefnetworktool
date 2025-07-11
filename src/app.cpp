#include <filesystem>
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "app.h"
#include "windowdelegate.h"
#include "viewdelegate.h"
#include "pagehandler.h"
#include "v8handler.h"

CefRefPtr<CefRenderProcessHandler> App::GetRenderProcessHandler()
{
    return this;
}

void App::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    CefRefPtr<CefV8Value> global = context->GetGlobal();
    CefRefPtr<V8Handler> handler = new V8Handler();

    CefRefPtr<CefV8Value> getIps = CefV8Value::CreateFunction("getIps", handler);
    global->SetValue("getIps", getIps, V8_PROPERTY_ATTRIBUTE_NONE);

    CefRefPtr<CefV8Value> startNetwork = CefV8Value::CreateFunction("startNetwork", handler);
    global->SetValue("startNetwork", startNetwork, V8_PROPERTY_ATTRIBUTE_NONE);
}

void App::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
	auto url = "file://" + std::filesystem::current_path().append("index.html").string();
    CefBrowserSettings settings;
    CefRefPtr<PageHandler> pageHandler(new PageHandler());
    CefRefPtr<ViewDelegate> viewDelegate(new ViewDelegate());
    CefRefPtr<CefBrowserView> browserView = CefBrowserView::CreateBrowserView(pageHandler, url, settings, nullptr, nullptr, viewDelegate);
    CefWindow::CreateTopLevelWindow(new WindowDelegate(browserView));
}
void App::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
    //command_line->AppendSwitchWithValue("--single-process", "1");
	command_line->AppendSwitch("--force-dark-mode");
	command_line->AppendSwitchWithValue("--allow-file-access-from-files", "1");
}