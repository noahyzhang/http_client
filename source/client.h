//
// Created by noahyzhang on 2021/9/30.
//

#ifndef HTTP_CLIENT_CLIENT_H
#define HTTP_CLIENT_CLIENT_H

#include <memory>
#include <curl/curl.h>
#include "http_client/http_client.h"

namespace HttpClient {

    class Client {
    public:
        Client() noexcept;
        ~Client() noexcept;
        Client(const Client& other) = delete;
        Client& operator=(const Client& other) = delete;
        Client(Client&& other) noexcept ;
        Client& operator=(Client&& other) noexcept ;

    public:
        bool InitClient();
        void SetHttpHeaderFields(std::map<std::string, std::string>&& header_fields);
        void AppendHeaderFields(const std::string& key, const std::string& value);
        bool SetConnectTimeout(size_t timeout_s);
        bool SetEntireRequestTimeout(size_t timeout_s);
        std::unique_ptr<Response> Get(const char* url);
        std::unique_ptr<Response> Post(const char* url, const std::string& data);

    private:
        std::unique_ptr<Response> ExecuteRequest(const char* url);
        static size_t WriteCallBack(char* ptr, size_t size, size_t nmemb, void* user_data);
        static size_t HeaderCallBack(char* buffer, size_t size, size_t nitems, void* user_data);

    private:
        CURL* curl_handler_;
        std::map<std::string, std::string> header_fields_;
    };
}

#endif //HTTP_CLIENT_CLIENT_H
