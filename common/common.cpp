//
// Created by mingj on 19-2-28.
//

#include "common.h"

namespace mingj {
    namespace sock {
        int WrapSockFun(const std::function<int()> &fun) {
            int res;
            while ((res = fun()) < 0) {
                if (errno != EINTR)
                    break;
            }
            return res;
        }

        /**
         * 在一个套接字中读取n字节
         * @param fd
         * @param vptr
         * @param n
         * @return
         */
        ssize_t Readn(int fd, char *vptr, size_t n) {
            size_t nleft;
            ssize_t nread;
            char *ptr;
            ptr = vptr;
            nleft = n;
            while (nleft > 0) {
                nread = read(fd, ptr, nleft);
                if (nread < 0) {
                    if (errno == EINTR)
                        nread = 0; /* and call read() again */
                    else {
                        std::cerr << errno << std::endl;
                        return (-1);
                    }
                } else if (nread == 0)
                    break; /*EOF*/
                nleft -= nread;
                ptr += nread;
            }
            return (n - nleft); /* return >= 0 */
        }

        ssize_t Writen(int fd, const char *vptr, size_t n) {
            size_t nleft;
            ssize_t nwritten;
            const char *ptr;
            ptr = vptr;
            nleft = n;
            while (nleft > 0) {
                if ((nwritten = write(fd, ptr, nleft)) <= 0) {
                    if (nwritten < 0 && errno == EINTR)
                        nwritten = 0; /* and call write() again */
                    else {
                        std::cerr << errno << std::endl;
                        return (-1); /* error */
                    }
                }
                nleft -= nwritten;
                ptr += nwritten;
            }
            return n;
        }

        int Write(int sockfd, const char *buf, unsigned int len) {
            return WrapSockFun([=] {
                return write(sockfd, buf, len);
            });
        }

        int Read(int sockfd, char *buf, unsigned int maxSize) {
            return WrapSockFun([=] {
                return read(sockfd, buf, maxSize);
            });
        }

        int Socket(int domain, int type, int protocol) {
            return WrapSockFun([=] {
                return socket(domain, type, protocol);
            });
        }

        int Bind(int sockfd, SA *servaddr, socklen_t len) {
            return WrapSockFun([=] {
                return bind(sockfd, servaddr, len);
            });
        }

        int Listen(int sockfd, int n) {
            return WrapSockFun([=] {
                return listen(sockfd, n);
            });
        }

        int Accept(int sockfd, SA *servaddr, socklen_t *len) {
            return WrapSockFun([=] {
                return accept(sockfd, servaddr, len);
            });
        }

        int Connect(int sockfd, SA *servaddr, socklen_t len) {
            return WrapSockFun([=] {
                return connect(sockfd, servaddr, len);
            });
        }

        int Shutdown(int sockfd, int how) {
            return WrapSockFun([=] {
                return shutdown(sockfd, how);
            });
        }

        /**
         * 往sockfd套接字指向的对端主机发送一串字符串
         * @param sockfd
         * @param msg
         */
        void sendString(int sockfd, const std::string &msg) {
            Write(sockfd, msg.c_str(), static_cast<unsigned int>(msg.size()));
        }

        std::string recvString(int sockfd, char *buf, unsigned int maxSize) {
            int bytes = Read(sockfd, buf, maxSize);
            cout << bytes << endl;
            if (bytes >= 0)
                return std::string(buf, static_cast<unsigned long>(bytes));
            else
                return "";
        }


        void easySuccess(int sockfd, const string &msg, unsigned int fileSize, unsigned int chunkSize) {
            ResponseBody responseBody(ProtocolHelper::RESPONSE_CODE_SUCCESS, msg, fileSize, chunkSize);
            sendString(sockfd, responseBody.toJson());
        }

        void easyErr(int sockfd, int code) {
            ResponseBody responseBody(code, mingj::protocol::error_code::codeToErrMsg(code));
            sendString(sockfd, responseBody.toJson());
            close(sockfd);
        }
    }
}
