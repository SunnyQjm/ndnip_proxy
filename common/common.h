//
// Created by mingj on 19-2-28.
//

#ifndef NDNIP_PROXY_COMMON_H
#define NDNIP_PROXY_COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <functional>
#include <iostream>
#include <string>
#include "protocol.h"

typedef struct sockaddr SA;


namespace mingj {
    namespace sock {
        int WrapSockFun(const std::function<int()> &fun);
        ssize_t Readn(int fd, char *vptr, size_t n);
        ssize_t Writen(int fd, const char *vptr, size_t n);
        int Write(int sockfd, const char *buf, unsigned int len);
        int Read(int sockfd, char *buf, unsigned int maxSize);

        int Socket(int domain, int type, int protocol);
        int Bind(int sockfd, SA *servaddr, socklen_t len);
        int Listen(int sockfd, int n);
        int Accept(int sockfd, SA *servaddr, socklen_t *len);
        int Connect(int sockfd, SA *servaddr, socklen_t len);
        int Shutdown(int sockfd, int how);
        /**
         * 往sockfd套接字指向的对端主机发送一串字符串
         * @param sockfd
         * @param msg
         */
        void sendString(int sockfd, const std::string &msg);
        std::string recvString(int sockfd, char *buf, unsigned int maxSize);



        void easySuccess(int sockfd, const string &msg, unsigned int fileSize, unsigned int chunkSize);

        void easyErr(int sockfd, int code);
    }
}

#endif //NDNIP_PROXY_COMMON_H
