//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPSERVERHELPER_H
#define NDNIP_PROXY_BOOSTTCPSERVERHELPER_H

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include "common.h"

using namespace mingj::sock;

class BoostTCPServerHelper {
public:
    explicit BoostTCPServerHelper(in_port_t port, unsigned int buffer_size = 8000);

    void startListen();

    void deal(int connfd);

private:
    int listenFd;
    unsigned short port;
    char buf[10000];
    unsigned int buffer_size;
};


#endif //NDNIP_PROXY_BOOSTTCPSERVERHELPER_H
