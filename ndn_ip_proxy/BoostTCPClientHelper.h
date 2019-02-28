//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
#define NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H

#include "common.h"

using namespace std;
using namespace mingj::sock;
class BoostTCPClientHelper {

public:
    BoostTCPClientHelper(const std::string &ip, in_port_t port, int buffer_size = 8000);
    ~BoostTCPClientHelper();

    /**
     * 连接
     */
    void connect();

    /**
     * 从远程服务器获取文件
     * @param fileName
     */
    void getFileFromServer(std::string fileName);

private:
    int sockfd;
    struct sockaddr_in servaddr{};
    char buf[10000];
    unsigned int buffer_size;
};


#endif //NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
