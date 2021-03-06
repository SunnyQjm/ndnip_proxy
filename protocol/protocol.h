//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_PROTOCOL_H
#define NDNIP_PROXY_PROTOCOL_H

#include "RequestBody.h"
#include "ResponseBody.h"
#include "ProtocolHelper.h"

namespace mingj {
    namespace protocol {
        namespace error_code {
            const int ERR_FILE_NOT_EXIST = -1;
            const int ERR_NOT_SUPPORT_CODE = -2;
            const int ERR_FILE_COULD_NOT_OPEN = -3;
            const int ERR_READ_END_OF_FILE = -4;


            std::string codeToErrMsg(int errcode);
        }
    }
}
#endif //NDNIP_PROXY_PROTOCOL_H
