//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
#define NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H

#include "common.h"

class BoostTCPClientHelper {

public:
    explicit BoostTCPClientHelper(const std::string &ip, unsigned short port, size_t buffer_size = 8000);
    ~BoostTCPClientHelper();

    /**
     * 连接
     */
    void connect();

    /**
     * 从远程服务器获取文件
     * @param fileName
     */
    void getFileFromServer(const std::string& fileName, function<void(ResponseBody &)> onResponse,
            function<void(uint8_t *, size_t, int)> callback);

    /**
     * 从远程服务器获取文件的一片数据
     */
    void getFileSliceFromServer(const std::string& fileName, int sliceNum, function<void(ResponseBody &)> onResponse,
                                function<void(uint8_t *, size_t)> callback);

    void testBlockChainRequest();
private:
    io_service service;
    ip::tcp::endpoint ep;
    ip::tcp::socket *sock = nullptr;
    char buf[10000];
    size_t buffer_size;


};


#endif //NDNIP_PROXY_BOOSTTCPCLIENTHELPER_H
