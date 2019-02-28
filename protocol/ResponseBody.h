//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_RESPONSEBODY_H
#define NDNIP_PROXY_RESPONSEBODY_H

#include <utility>
#include <string>
#include "BaseProtocolItem.h"

class ResponseBody : public BaseProtocolItem {
public:
    ResponseBody(int code, std::string data, unsigned int fileSize = 0, unsigned int chunkSize = 0) :
            BaseProtocolItem(code, std::move(data)), fileSize(fileSize), chunkSize(chunkSize) {
    }

    std::string toJson() const override {
        Json::Value root;
        root[KEY_CODE] = code;
        root[KEY_DATA] = data;
        root[KEY_FILE_SIZE] = fileSize;
        root[KEY_CHUNK_SIZE] = chunkSize;
        return root.toStyledString();
    }

    unsigned int fileSize;
    unsigned int chunkSize;
};


#endif //NDNIP_PROXY_RESPONSEBODY_H
