//
// Created by noahyzhang on 2021/9/30.
//

#include "client.h"
#include "string_util.h"
#include "constants.h"

namespace HttpClient {

    Client::Client() noexcept : curl_handler_(nullptr) {}

    Client::~Client() noexcept {
        if (curl_handler_) {
            curl_easy_cleanup(curl_handler_);
        }
        curl_handler_ = nullptr;
        header_fields_.clear();
    }

    Client::Client(Client &&other) noexcept {
        curl_handler_ = other.curl_handler_;
        header_fields_ = std::move(other.header_fields_);
        other.curl_handler_ = nullptr;
    }

    Client& Client::operator=(Client &&other) noexcept {
        curl_handler_ = other.curl_handler_;
        header_fields_ = std::move(other.header_fields_);
        other.curl_handler_ = nullptr;
        return *this;
    }

    bool Client::InitClient() {
        curl_handler_ = curl_easy_init();
        if (!curl_handler_) {
            return false;
        }
        return true;
    }

    void Client::SetHttpHeaderFields(std::map<std::string, std::string> &&header_fields) {
        header_fields_ = header_fields;
    }

    void Client::AppendHeaderFields(const std::string &key, const std::string &value) {
        header_fields_[key] = value;
    }

    bool Client::SetConnectTimeout(size_t timeout_s) {
        if (CURLE_OK == curl_easy_setopt(curl_handler_, CURLOPT_CONNECTTIMEOUT, timeout_s)
            && CURLE_OK == curl_easy_setopt(curl_handler_, CURLOPT_NOSIGNAL, 1)) {
            return true;
        }
        return false;
    }

    bool Client::SetEntireRequestTimeout(size_t timeout_s) {
        if (CURLE_OK == curl_easy_setopt(curl_handler_, CURLOPT_TIMEOUT, timeout_s)
            && CURLE_OK == curl_easy_setopt(curl_handler_, CURLOPT_NOSIGNAL, 1)) {
            return true;
        }
        return false;
    }

    std::unique_ptr<Response> Client::Get(const char* url) {
        return ExecuteRequest(url);
    }

    std::unique_ptr<Response> Client::Post(const char* url, const std::string& data) {
        curl_easy_setopt(curl_handler_, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_handler_, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl_handler_, CURLOPT_POSTFIELDSIZE, data.size());
        return ExecuteRequest(url);
    }

    std::unique_ptr<Response> Client::ExecuteRequest(const char* url) {
        auto response = std::unique_ptr<Response>(new Response);

        curl_easy_setopt(curl_handler_, CURLOPT_URL, url);
        curl_easy_setopt(curl_handler_, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl_handler_, CURLOPT_WRITEDATA, response.get());
        curl_easy_setopt(curl_handler_, CURLOPT_HEADERFUNCTION, HeaderCallBack);
        curl_easy_setopt(curl_handler_, CURLOPT_HEADERDATA, response.get());

        curl_slist* curl_header_list = nullptr;
        std::string tmp_header_string;
        for (auto & header_field : header_fields_) {
            tmp_header_string = header_field.first;
            tmp_header_string += ": ";
            tmp_header_string += header_field.second;
            curl_header_list = curl_slist_append(curl_header_list, tmp_header_string.c_str());
        }
        curl_easy_setopt(curl_handler_, CURLOPT_CONNECTTIMEOUT, Constants::DefaultConnectTimeoutSecond);
        curl_easy_setopt(curl_handler_, CURLOPT_TIMEOUT, Constants::DefaultEntireRequestTimeout);
        curl_easy_setopt(curl_handler_, CURLOPT_NOSIGNAL, 1);

        CURLcode ret_code;
        ret_code = curl_easy_perform(curl_handler_);
        if (CURLE_OK != ret_code) {
            response->code_ = ret_code;
            response->body_ = curl_easy_strerror(ret_code);
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl_handler_, CURLINFO_RESPONSE_CODE, &http_code);
            response->code_ = static_cast<int>(http_code);
        }
        curl_slist_free_all(curl_header_list);
        curl_easy_reset(curl_handler_);
        return response;
    }

    size_t Client::WriteCallBack(char *ptr, size_t size, size_t nmemb, void *user_data) {
        auto resp = reinterpret_cast<Response*>(user_data);
        resp->body_.append(reinterpret_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    }

    size_t Client::HeaderCallBack(char *buffer, size_t size, size_t nitems, void *user_data) {
        auto resp = reinterpret_cast<Response*>(user_data);
        std::string header(buffer, size * nitems);
        size_t separator_location = header.find_first_of(':');
        if (std::string::npos == separator_location) {
            if (0 == header.length()) {
                return size * nitems;
            }
            string_trim(header);
            resp->headers_[header] = "";
        } else {
            std::string header_key = header.substr(0, separator_location);
            string_trim(header_key);
            std::string header_value = header.substr(separator_location+1);
            string_trim(header_value);
            resp->headers_[header_key] = header_value;
        }
        return size * nitems;
    }
}
