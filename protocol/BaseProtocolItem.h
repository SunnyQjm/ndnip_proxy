//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BASEPROTOCOLITEM_H
#define NDNIP_PROXY_BASEPROTOCOLITEM_H


#include <jsoncpp/json/json.h>

class BaseProtocolItem {
public:

    //KEY
    static std::string KEY_CODE;
    static std::string KEY_DATA;
    static std::string KEY_FILE_SIZE;
    static std::string KEY_CHUNK_SIZE;
    static std::string KEY_SLICE_NUM;

    BaseProtocolItem(int code, std::string data): code(code), data(std::move(data)){}
    int code;
    std::string data;

    virtual std::string toJson() const {
        Json::Value root;
        root[KEY_CODE] = code;
        root[KEY_DATA] = data;
        return root.toStyledString();
    };
};


#endif //NDNIP_PROXY_BASEPROTOCOLITEM_H
