//
// Created by mingj on 19-2-27.
//

#include <iostream>
#include "ProtocolHelper.h"

int ProtocolHelper::REQUEST_CODE_FILE = 1;
int ProtocolHelper::REQUEST_CODE_FILE_SLICE = 2;

int ProtocolHelper::RESPONSE_CODE_SUCCESS = 0;


ResponseBody ProtocolHelper::jsonToResponseBody(const std::string &json) {
    JSONCPPHelper jsoncppHelper(json);
    return ResponseBody(jsoncppHelper.getInt(BaseProtocolItem::KEY_CODE),
                        jsoncppHelper.getString(BaseProtocolItem::KEY_DATA),
                        static_cast<unsigned int>(jsoncppHelper.getInt(BaseProtocolItem::KEY_FILE_SIZE)),
                        static_cast<unsigned int>(jsoncppHelper.getInt(BaseProtocolItem::KEY_CHUNK_SIZE)));
}

RequestBody ProtocolHelper::jsonToRequestBody(const std::string &json) {
    JSONCPPHelper jsoncppHelper(json);
    return RequestBody(jsoncppHelper.getInt(BaseProtocolItem::KEY_CODE),
                       jsoncppHelper.getString(BaseProtocolItem::KEY_DATA),
                       jsoncppHelper.getInt(BaseProtocolItem::KEY_SLICE_NUM));
}

const std::string ProtocolHelper::itemToJson(const BaseProtocolItem &baseProtocolItem) {
    return baseProtocolItem.toJson();
}

