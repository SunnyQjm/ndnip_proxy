//
// Created by mingj on 19-2-28.
//

#include "common.h"

namespace mingj {
    namespace sock {

        size_t read_complete(char *buf, const boost::system::error_code &err, size_t bytes) {
            if (err) return 0;
            bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
            // 我们一个一个读取直到读到回车，不缓存
            return found ? 0 : 1;
        }

        size_t sendStr(ip::tcp::socket *sock, const string &msg) {
            return sock->write_some(buffer(msg));
        }

        string readStr(ip::tcp::socket *sock, char *buf, size_t len) {
            auto bytes = sock->read_some(buffer(buf, len));
            return string(buf, bytes);
        }

        size_t readLine(ip::tcp::socket *sock, char *buf, size_t len) {
            return read(*sock, buffer(buf, len), boost::bind(read_complete, buf, _1, _2));
        }

        size_t readn(ip::tcp::socket *sock, char *buf, size_t len) {
            return read(*sock, buffer(buf, len));
        }

        size_t writen(ip::tcp::socket *sock, char *buf, size_t len) {
            return write(*sock, buffer(buf, len));
        }

    }
}
