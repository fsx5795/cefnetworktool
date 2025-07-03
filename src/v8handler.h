#pragma once

#ifdef _WIN32
    #include <WinSock2.h>
#endif
#include <unordered_map>
#include "include/cef_v8.h"

class V8Handler : public CefV8Handler
{
public:
    virtual bool Execute(const CefString&, CefRefPtr<CefV8Value>, const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&) override;

private:
    bool start_tcp(std::string_view, unsigned short, CefString&) const;
    void run_tcp(std::string_view, unsigned short) const;
    IMPLEMENT_REFCOUNTING(V8Handler);
#ifdef WIN32
    mutable SOCKET fd{INVALID_SOCKET};
#else
    mutable int fd{-1};
#endif
    mutable bool running{true};
    mutable std::unordered_map<int, std::string> clients;
};
