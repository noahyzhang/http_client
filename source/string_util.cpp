//
// Created by noahyzhang on 2021/9/30.
//

#include "string_util.h"

static inline std::string& string_trim(std::string& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) {
        return !std::isspace(c);
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) {
        return !std::isspace(c);
    }).base(), str.end());
    return str;
}
