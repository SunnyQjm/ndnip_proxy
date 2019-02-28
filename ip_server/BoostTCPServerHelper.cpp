//
// Created by mingj on 19-2-27.
//

#include "BoostTCPServerHelper.h"
#include "FileUtils.h"

using namespace std;

BoostTCPServerHelper::BoostTCPServerHelper(in_port_t port, unsigned int buffer_size) :
        port(port),
        buffer_size(buffer_size) {
    listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr{};
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    Bind(listenFd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenFd, 20);
}

void BoostTCPServerHelper::startListen() {
    // 启动监听
    for (;;) {

        struct sockaddr_in cliaddr{};
        socklen_t clilen;
        int connfd = Accept(listenFd, (SA *) &cliaddr, &clilen);

        // 在子线程中处理请求
        boost::thread t([=]() {
            this->deal(connfd);
        });
    }
}

void BoostTCPServerHelper::deal(int connfd) {
    // 首先传递过来的是请求数据，根据请求，选择合适的文件进行传输

    std::string request(std::move(recvString(connfd, this->buf, this->buffer_size)));
    RequestBody requestBody = ProtocolHelper::jsonToRequestBody(request);

    cout << "receive: " << requestBody.toJson() << endl;

    if (requestBody.code == ProtocolHelper::REQUEST_CODE_FILE) {     //处理文件传输
        boost::filesystem::path filePath = FileUtils::getResourcePath();

        cout << filePath.string() << ": " << boost::filesystem::exists(filePath) << endl;
        // 得到文件的绝对路径
        filePath.append(requestBody.data);

        // 判断文件是否存在，且是一个文件而不是目录
        if (boost::filesystem::exists(filePath)) {
            // 存在则进行文件传输操作

            // 首先告知客户端，文件存在。准备进行文件传输
            auto fileSize = boost::filesystem::file_size(filePath);
            easySuccess(connfd, "success", static_cast<unsigned int>(fileSize), this->buffer_size);

            //等客户端回复开始传输文件
            recvString(connfd, this->buf, this->buffer_size);
            
            boost::filesystem::fstream fs(filePath, std::ios_base::binary | std::ios_base::in);
            if (fs.is_open()) {      //打开文件成功
                cout << "begin trans: " << requestBody.data << endl;

                int total = 0;
                int slice = 0;
                while (!fs.eof()) {              //读文件
                    fs.read(buf, buffer_size);
                    auto count = fs.gcount();
                    total += count;
                    auto sendBytes = Writen(connfd, this->buf, static_cast<size_t>(count));
                    slice++;
                    cout << "slice: " << slice << " -> " << sendBytes << endl;
                }

//                cout << "total: " << total << endl;
//                //关闭文件
//                fs.close();
//


                // 等待客户端确认接收完毕
                recvString(connfd, this->buf, this->buffer_size);

                //关闭socket写一端
                Shutdown(connfd, SHUT_WR);
//                close(connfd);
//                sockPtr->close();
                // 关闭socket
//                close(sockPtr);
            } else {
                cout << "file not exists: " << filePath.string() << endl;
                easyErr(connfd, mingj::protocol::error_code::ERR_FILE_COULD_NOT_OPEN);
            }
        } else {
            // 不存在则返回错误信息进行提示
            easyErr(connfd, mingj::protocol::error_code::ERR_FILE_NOT_EXIST);
        }
    } else {
        easyErr(connfd, mingj::protocol::error_code::ERR_NOT_SUPPORT_CODE);
    }
}
