//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_PROTOCOLHELPER_H
#define NDNIP_PROXY_PROTOCOLHELPER_H

#include "RequestBody.h"
#include "ResponseBody.h"
#include "JSONCPPHelper.h"

class ProtocolHelper {

public:

    //Request code
    static int REQUEST_CODE_FILE;

    //Response code
    static int RESPONSE_CODE_SUCCESS;


    /**
     * json 转ResponseBody对象
     * @param json
     * @return
     */
    static ResponseBody jsonToResponseBody(const std::string& json);

    static RequestBody jsonToRequestBody(const std::string& json);

    static const std::string itemToJson(const BaseProtocolItem &baseProtocolItem) ;
};


#endif //NDNIP_PROXY_PROTOCOLHELPER_H
