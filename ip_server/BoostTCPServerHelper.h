//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPSERVERHELPER_H
#define NDNIP_PROXY_BOOSTTCPSERVERHELPER_H

#include "common.h"
#include <boost/filesystem.hpp>

using namespace boost::asio;
using namespace boost::system;

class BoostTCPServerHelper {
public:
    explicit BoostTCPServerHelper(unsigned short port, unsigned int buffer_size = 8000);

    void startListen();

    void easyErr(ip::tcp::socket *sock, int code);

    void easySuccess(ip::tcp::socket *sock, const std::string& msg = "success", unsigned int fileSize = 0,
            unsigned int chunkSize = 0);

    void deal(ip::tcp::socket *sockPtr);

    void close(ip::tcp::socket *sockPtr);

private:
    io_service service;
    unsigned short port;
    char buf[10000];
    unsigned int buffer_size;
};


#endif //NDNIP_PROXY_BOOSTTCPSERVERHELPER_H