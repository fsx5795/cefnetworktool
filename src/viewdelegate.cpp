#include "viewdelegate.h"
#include "windowdelegate.h"
// 当前页面弹出新窗口时此方法被执行
bool ViewDelegate::OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browserView, CefRefPtr<CefBrowserView> popupBrowserView, bool isDevtools)
{
    CefWindow::CreateTopLevelWindow(new WindowDelegate(popupBrowserView));
    return true;
}