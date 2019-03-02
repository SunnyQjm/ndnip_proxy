
//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_REQUESTBODY_H
#define NDNIP_PROXY_REQUESTBODY_H

#include <string>
#include <utility>
#include "BaseProtocolItem.h"
#include <jsoncpp/json/json.h>

class RequestBody : public BaseProtocolItem {
public:
    RequestBody(int code, std::string data, int sliceNum = -1)
            : BaseProtocolItem(code, std::move(data)), sliceNum(sliceNum) {}

    std::string toJson() const override {
        Json::Value root;
        root[KEY_CODE] = code;
        root[KEY_DATA] = data;
        root[KEY_SLICE_NUM] = sliceNum;
        return root.toStyledString();
    }

    int sliceNum;

};


#endif //NDNIP_PROXY_REQUESTBODY_H
