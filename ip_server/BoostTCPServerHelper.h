//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPSERVERHELPER_H
#define NDNIP_PROXY_BOOSTTCPSERVERHELPER_H

#define BOOST_ASIO_HAS_MOVE
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::system;

class BoostTCPServerHelper {
public:
    BoostTCPServerHelper(in_port_t port, unsigned int buffer_size = 8000);

    void startListen();

    std::string recvStr(ip::tcp::socket *sock);

    void writeStr(ip::tcp::socket *sock, const std::string& msg);

    void easyErr(ip::tcp::socket *sock, int code);

    void easySuccess(ip::tcp::socket *sock, const std::string& msg = "success", int fileSize = 0, int chunkSize = 0);

    void deal(ip::tcp::socket *sockPtr);

    void close(ip::tcp::socket *sockPtr);

private:
    io_service service;
    unsigned short port;
    char buf[10000];
    unsigned int buffer_size;
};


#endif //NDNIP_PROXY_BOOSTTCPSERVERHELPER_H
