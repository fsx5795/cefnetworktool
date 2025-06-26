#include "pagehandler.h"
#include "include/wrapper/cef_helpers.h"
//页面创建成功
void PageHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();    
    browsers.push_back(browser);
}
//页面即将关闭
void PageHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    std::list<CefRefPtr<CefBrowser>>::iterator bit = browsers.begin();
    for (; bit != browsers.end(); ++bit) {
        if ((*bit)->IsSame(browser)) {
            browsers.erase(bit);
            break;
        }
    }
    if (browsers.empty()) {
        CefQuitMessageLoop();
    }
}