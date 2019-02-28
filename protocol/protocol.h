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


            std::string codeToErrMsg(int errcode) {
                switch (errcode) {
                    case ERR_FILE_NOT_EXIST:
                        return "文件不存在";
                    case ERR_NOT_SUPPORT_CODE:
                        return "不支持的协议码（code）";
                    case ERR_FILE_COULD_NOT_OPEN:
                        return "文件打开失败";
                    default:
                        return "未知错误";
                }
            }
        }
    }
}
#endif //NDNIP_PROXY_PROTOCOL_H
