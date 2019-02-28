//
// Created by mingj on 19-2-28.
//

#ifndef NDNIP_PROXY_COMMON_H
#define NDNIP_PROXY_COMMON_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "string"
#include <iostream>
#include "protocol.h"


using namespace std;
using namespace boost::asio;
using namespace boost::system;

namespace mingj {
    namespace sock {

        /**
         * 发送一个字符串（字符串不要包含\n),
         * @param sock
         * @param msg
         * @return
         */
        size_t sendStr(ip::tcp::socket *sock, const string& msg);

        /**
         * 接收一个字符串
         * @param sock
         * @param buf
         * @param len
         * @return
         */
        string readStr(ip::tcp::socket *sock, char* buf, size_t len);

        /**
         * 接收一行数据
         * @param sock
         * @param buf
         * @param len
         * @return
         */
        size_t readLine(ip::tcp::socket *sock, char* buf, size_t len);

        /**
         * 读取n个字节
         * @param sock
         * @param buf
         * @param len
         * @return
         */
        size_t readn(ip::tcp::socket *sock, char *buf, size_t len);

        /**
         * 写n个字节
         * @param sock
         * @param buf
         * @param len
         * @return
         */
        size_t writen(ip::tcp::socket *sock, char *buf, size_t len);
    }
}

using  namespace mingj::sock;
using namespace mingj::protocol::error_code;
#endif //NDNIP_PROXY_COMMON_H
