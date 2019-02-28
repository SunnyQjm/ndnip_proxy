
//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_REQUESTBODY_H
#define NDNIP_PROXY_REQUESTBODY_H
#include <string>
#include <utility>
#include "BaseProtocolItem.h"
#include <jsoncpp/json/json.h>

class RequestBody: public BaseProtocolItem {
public:
    RequestBody(int code, std::string data): BaseProtocolItem(code, data){}
};


#endif //NDNIP_PROXY_REQUESTBODY_H
