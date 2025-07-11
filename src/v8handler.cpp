#ifdef WIN32
	//#define _WIN32_WINNT 0x0600
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif
#include <thread>
#include "v8handler.h"

void V8Handler::run_tcp(std::string_view ip, unsigned short port) const
{
    sockaddr_in client;
    while (running) {
        socklen_t clientLen = sizeof client;
        int csd = accept(fd, reinterpret_cast<sockaddr*>(&client), &clientLen);
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        CefRefPtr<CefFrame> frame = context->GetBrowser()->GetMainFrame();
        frame->ExecuteJavaScript("console.log('+++++++++++++')", frame->GetURL(), 0);
        if (csd >= 0) {
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client.sin_addr, ipstr, INET_ADDRSTRLEN);
            this->clients.emplace(csd, ipstr);
            unsigned short port = client.sin_port;
            /*
            netLink.BlockingCall([&ipstr, port] (Napi::Env env, Napi::Function callback) {
                callback.Call({Napi::Boolean::New(env, false), Napi::String::New(env, ipstr), Napi::Number::New(env, port)});
            });
            */

            CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
            CefRefPtr<CefV8Value> global = context->GetGlobal();
            CefRefPtr<CefV8Value> jsFunc = global->GetValue("netLink");
            if (jsFunc && jsFunc->IsFunction()) {
                
                CefV8ValueList args;
                args.push_back(CefV8Value::CreateBool(false));
                args.push_back(CefV8Value::CreateString(ipstr));
                args.push_back(CefV8Value::CreateInt(port));
                
                CefRefPtr<CefV8Value> jsResult = jsFunc->ExecuteFunction(nullptr, args);
                
                if (jsResult && jsResult->IsString()) {
                    CefString result = jsResult->GetStringValue();
                    printf("JS returned: %s\n", result.ToString().c_str());
                }
                
            }
            //std::thread(recv_msg, csd, std::cref(env), ipstr, client.sin_port).detach();
        }
    }
}

bool V8Handler::start_tcp(std::string_view ip, unsigned short port, CefString &exception) const
{
#ifdef WIN32
    if (this->fd == INVALID_SOCKET) {
#else
    if (this->fd < 0) {
#endif
    CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
    CefRefPtr<CefFrame> frame = context->GetBrowser()->GetMainFrame();
    frame->ExecuteJavaScript("console.log('11111111111')", frame->GetURL(), 0);
        this->running = true;
        this->fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            exception = strerror(errno);
            return false;
        }
    frame->ExecuteJavaScript("console.log('22222222222')", frame->GetURL(), 0);
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, ip.data(), &server.sin_addr.s_addr);
        if (bind(fd, reinterpret_cast<sockaddr*>(&server), sizeof server) < 0) {
#ifdef WIN32
            closesocket(fd);
#else
            close(fd);
#endif
            exception = strerror(errno);
            return false;
        }
    frame->ExecuteJavaScript("console.log('33333333333333')", frame->GetURL(), 0);
        if (listen(fd, 8) < 0) {
#ifdef WIN32
            closesocket(fd);
#else
            close(fd);
#endif
            exception = strerror(errno);
            return false;
        }
    frame->ExecuteJavaScript("console.log('444444444444')", frame->GetURL(), 0);
        std::thread(&V8Handler::run_tcp, this, ip, port).detach();
        return true;
    }
    return false;
}

bool V8Handler::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
    if (name.ToString().compare("getIps") == 0) {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            exception = "WSAStartup failed";
            return false;
        }
#endif
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        CefRefPtr<CefFrame> frame = context->GetBrowser()->GetMainFrame();
        frame->ExecuteJavaScript("console.log('+++++++++++++')", frame->GetURL(), 0);
        char buf[100] = { '\0' };
        std::vector<std::string> ips;
        if (gethostname(buf, sizeof(buf)) == 0) {
            struct addrinfo hints, *res;
            memset(&hints, '\0', sizeof(addrinfo));
            hints.ai_family = AF_INET;
            auto ret = getaddrinfo(buf, nullptr, &hints, &res);
            if (ret == 0) {
                char host[256];
                for (addrinfo *tmp = res; tmp != nullptr; tmp = tmp->ai_next) {
                    getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof host, nullptr, 0, NI_NUMERICHOST);
                    if (strcmp("127.0.0.1", host) == 0)
                        continue;
                    ips.push_back(host);
                }
            }
        }
        
#ifdef _WIN32
        WSACleanup();
#endif
        retval = CefV8Value::CreateArray(ips.size());
        for (auto i{0}; i < ips.size(); ++i)
            retval->SetValue(i, CefV8Value::CreateString(ips.at(i)));
        return true;
    } else if (name.ToString().compare("startNetwork") == 0) {
        if (arguments.size() < 3 || !arguments.at(0)->IsString() || !arguments.at(1)->IsInt() || !arguments.at(2)->IsString()) {
            exception = "arg type is error!";
            return false;
        }

        std::string ip = arguments.at(0)->GetStringValue().ToString();
        uint16_t port = arguments.at(1)->GetUIntValue();
        std::string type = arguments.at(2)->GetStringValue().ToString();
        if (type.compare("tcp") == 0)
            return this->start_tcp(ip, port, exception);
    }
    return false;
}
