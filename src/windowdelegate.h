#pragma once
#include "include/views/cef_window.h"
#include "include/views/cef_browser_view.h"
//窗口代理对象，CEF框架会把与窗口创建有关的逻辑交给这个对象来执行
class WindowDelegate : public CefWindowDelegate
{
public:
	//接收一个CefBrowserView类型的智能指针，并把这个智能指针存放到browserView私有变量中，以备后续使用
	explicit WindowDelegate(CefRefPtr<CefBrowserView> browserView) : browserView(browserView) {};
	void OnWindowCreated(CefRefPtr<CefWindow> window) override;
	void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
	//设置窗口位置和大小事件
	CefRect GetInitialBounds(CefRefPtr<CefWindow> window) override;
	WindowDelegate(const WindowDelegate&) = delete;
	WindowDelegate& operator=(const WindowDelegate&) = delete;
private:
	CefRefPtr<CefBrowserView> browserView;
	IMPLEMENT_REFCOUNTING(WindowDelegate);
};