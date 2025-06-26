#pragma once
#include "include/cef_app.h"
#include <list>
class PageHandler : public CefClient, public CefLifeSpanHandler
{
public:
    PageHandler() = default;
    //获取LifeSpanHandler对象
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    //页面创建成功
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    //页面即将关闭
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
private:
    IMPLEMENT_REFCOUNTING(PageHandler);
    std::list<CefRefPtr<CefBrowser>> browsers;
};