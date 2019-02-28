//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
#define NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H

#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "protocol.h"
#include <iostream>

using namespace boost::asio;
using namespace boost::system;
using namespace std;

class BoostTCPClientHelper {

public:
    BoostTCPClientHelper(const std::string &ip, size_t port, int buffer_size = 8000);
    ~BoostTCPClientHelper();

    /**
     * 连接
     */
    void connect();

    /**
     * 发送buf缓存重的len长度的字节, 返回error_code
     * @param buf
     * @param len
     */
    size_t send(char *buf, unsigned int len);

    size_t send(const std::string &str);


    /**
     *
     * @param buf
     * @param len
     * @return
     */
    size_t recv(char *buf, unsigned int len);

    std::string recvStr();



    /**
     * 从远程服务器获取文件
     * @param fileName
     */
    void getFileFromServer(const std::string& fileName);

    void close();
private:
    io_service service;
    ip::tcp::endpoint ep;
    ip::tcp::socket *sock = nullptr;
    char buf[10000];
    unsigned int buffer_size;
};


#endif //NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
