#include "include/cef_app.h"
#include "include/views/cef_display.h"
#include "windowdelegate.h"
//窗口创建成功
//当App类CreateTopLevelWindow方法被执行后，CEF框架将创建一个系统窗口，当这个窗口成功创建完成后，OnWindowCreated被调用
void WindowDelegate::OnWindowCreated(CefRefPtr<CefWindow> window)
{
    //我们在这个方法里把App类里创建的BrowserView对象添加到了这个窗口中
    window->AddChildView(this->browserView);
    //然后让这个窗口显示出来
    window->Show();
    //最后用户焦点被聚焦在BrowserView上
    this->browserView->RequestFocus();
    window->SetTitle(L"这是我的窗口标题");
    //把窗口移动到主屏幕中央的代码
    //window->CenterWindow(CefSize(800, 600));
}
//窗口销毁成功
void WindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window)
{
    this->browserView = nullptr;
    //这个方法会在CEF的消息循环中插入一个退出消息，CEF框架收到这个消息后，会退出消息循环，清理资源，退出应用
    CefQuitMessageLoop();
}
//设置窗口位置和大小
//当App类CreateTopLevelWindow方法被执行后，CEF框架将创建一个系统窗口，创建这个窗口之前，CEF框架会调用GetInitialBounds方法
//我们可以在窗口创建成功之后再通过窗口对象的CenterWindow方法来把窗口设置到屏幕正中间（同时也可以设置窗口尺寸），但这显然不如在窗口创建之初就明确窗口的位置和尺寸更高效。如果开发者不通过GetInitialBounds方法设置窗口的尺寸，还可以通过重写基类的GetPreferredSize方法来设置窗口尺寸。CEF的示例项目就是这么做的，但我认为还是在GetInitialBounds方法中完成这项工作比较好
CefRect WindowDelegate::GetInitialBounds(CefRefPtr<CefWindow> window)
{
    //通过CefDisplay类的静态方法GetPrimaryDisplay获取到了用户的主屏幕信息。
    CefRefPtr<CefDisplay> display = CefDisplay::GetPrimaryDisplay();
    //根据主屏幕信息及设定的窗口尺寸计算出窗口位于屏幕正中间时窗口的坐标。
    CefRect rect = display->GetBounds();
    //设定窗口的尺寸为宽 800 像素，高 600 像素。
    rect.x = (rect.width - 800) / 2;
    rect.y = (rect.height - 600) / 2;
    rect.width = 800;
    rect.height = 600;
    //通过CefRect结构把窗口的坐标及尺寸返回给CEF框架。
    return rect;
}