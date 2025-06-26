#pragma once
#include "include/cef_app.h"
#include "include/cef_render_process_handler.h"

class App : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
{
public:
	App() = default;
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
	void OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;
	void OnContextInitialized() override;
    void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;

private:
	IMPLEMENT_REFCOUNTING(App);
};