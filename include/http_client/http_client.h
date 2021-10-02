//
// Created by noahyzhang on 2021/9/30.
//

#ifndef HTTP_CLIENT_HTTP_CLIENT_H
#define HTTP_CLIENT_HTTP_CLIENT_H

#include <map>
#include <string>

namespace HttpClient {

    typedef struct {
        int code_;
        std::map<std::string, std::string> headers_;
        std::string body_;
    }Response;

    class HttpClient {
    public:
        HttpClient() noexcept;
        ~HttpClient() noexcept;
        HttpClient(const HttpClient& other) = delete;
        HttpClient& operator=(const HttpClient& other) = delete;
        HttpClient(HttpClient&& other) noexcept;
        HttpClient& operator=(HttpClient&& other) noexcept;

    public:
        bool InitHttpClient();
        void SetHttpHeaderFields(std::map<std::string, std::string>&& header_fields);
        void AppendHeaderFields(const std::string& key, const std::string& value);
        bool SetConnectTimeout(size_t timeout_s);
        bool SetEntireRequestTimeout(size_t timeout_s);
        std::unique_ptr<Response> Get(const char* url);
        std::unique_ptr<Response> Post(const char* url, const std::string& data);

    private:
        void* client_;
    };

}

#endif //HTTP_CLIENT_HTTP_CLIENT_H
