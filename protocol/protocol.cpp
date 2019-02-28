//
// Created by mingj on 19-2-28.
//

#include "protocol.h"

namespace mingj {
    namespace protocol {
        namespace error_code {

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