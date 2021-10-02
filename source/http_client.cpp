//
// Created by noahyzhang on 2021/9/30.
//

#include "http_client/http_client.h"
#include "client.h"

namespace HttpClient {

    HttpClient::HttpClient() noexcept {
        client_ = new Client();
    }

    HttpClient::~HttpClient() noexcept {
        if (client_) {
            delete (Client*)client_;
        }
        client_ = nullptr;
    }

    HttpClient::HttpClient(HttpClient &&other) noexcept {
        client_ = (Client*)other.client_;
        other.client_ = nullptr;
    }

    HttpClient& HttpClient::operator=(HttpClient &&other) noexcept {
        client_ = (Client*)other.client_;
        other.client_ = nullptr;
        return *this;
    }

    bool HttpClient::InitHttpClient() {
        return ((Client*)client_)->InitClient();
    }

    void HttpClient::SetHttpHeaderFields(std::map<std::string, std::string> &&header_fields) {
        ((Client*)client_)->SetHttpHeaderFields(std::move(header_fields));
    }

    void HttpClient::AppendHeaderFields(const std::string &key, const std::string &value) {
        ((Client*)client_)->AppendHeaderFields(key, value);
    }

    bool HttpClient::SetConnectTimeout(size_t timeout_s) {
        return ((Client*)client_)->SetConnectTimeout(timeout_s);
    }

    bool HttpClient::SetEntireRequestTimeout(size_t timeout_s) {
        return ((Client*)client_)->SetEntireRequestTimeout(timeout_s);
    }

    std::unique_ptr<Response> HttpClient::Get(const char *url) {
        return ((Client*)client_)->Get(url);
    }

    std::unique_ptr<Response> HttpClient::Post(const char *url, const std::string& data) {
        return ((Client*)client_)->Post(url, data);
    }
}