//
// Created by mingj on 19-2-27.
//

#include "BoostTCPServerHelper.h"
#include "FileUtils.h"


BoostTCPServerHelper::BoostTCPServerHelper(unsigned short port, unsigned int buffer_size) :
        port(port),
        buffer_size(buffer_size) {

}

void BoostTCPServerHelper::startListen() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), port));
    while (true) {
        auto *sockPtr = new ip::tcp::socket(service);
        acceptor.accept(*sockPtr);
        std::string s;
        boost::thread thread([=]() {
            cout << boost::this_thread::get_id() << endl;
            try {
                this->deal(sockPtr);
            } catch (exception &e) {
                cerr << e.what() << endl;
            }
        });
    }
}

void BoostTCPServerHelper::deal(ip::tcp::socket *sockPtr) {
    // 首先传递过来的是请求数据，根据请求，选择合适的文件进行传输

    std::string request(readStr(sockPtr, this->buf, this->buffer_size));
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
            easySuccess(sockPtr, "success", static_cast<int>(fileSize), this->buffer_size);
            boost::filesystem::fstream fs(filePath, std::ios_base::binary | std::ios_base::in);

            // 等待客户端写一行数据，标示可以开始传输文件
            readLine(sockPtr, this->buf, this->buffer_size);
            if (fs.is_open()) {      //打开文件成功

                int total = 0;
                while (!fs.eof()) {              //读文件
                    fs.read(buf, buffer_size);
                    auto count = fs.gcount();
                    size_t sendBytes = writen(sockPtr, this->buf, static_cast<size_t>(count));
                    total += sendBytes;
                }

                cout << "total: " << total << endl;
                //关闭文件
                fs.close();

                //关闭socket写一端
                sockPtr->shutdown(boost::asio::socket_base::shutdown_type::shutdown_send);

            } else {
                cout << "file not exists: " << filePath.string() << endl;
                easyErr(sockPtr, mingj::protocol::error_code::ERR_FILE_COULD_NOT_OPEN);
            }
        } else {
            // 不存在则返回错误信息进行提示
            easyErr(sockPtr, mingj::protocol::error_code::ERR_FILE_NOT_EXIST);
        }
    } else {
        easyErr(sockPtr, mingj::protocol::error_code::ERR_NOT_SUPPORT_CODE);
    }
}


void BoostTCPServerHelper::easyErr(ip::tcp::socket *sock, int code) {
    ResponseBody responseBody(code, mingj::protocol::error_code::codeToErrMsg(code));
    sendStr(sock, responseBody.toJson());
    close(sock);
}

void BoostTCPServerHelper::easySuccess(ip::tcp::socket *sock, const string &msg, unsigned int fileSize, unsigned int chunkSize) {
    ResponseBody responseBody(ProtocolHelper::RESPONSE_CODE_SUCCESS, msg, fileSize, chunkSize);
    sendStr(sock, responseBody.toJson());
}

void BoostTCPServerHelper::close(ip::tcp::socket *sockPtr) {
    if (nullptr != sockPtr) {
        sockPtr->close();
        delete sockPtr;
    }
}
