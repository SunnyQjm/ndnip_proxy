//
// Created by mingj on 19-2-27.
//

#include "BoostTCPServerHelper.h"
#include "FileUtils.h"
#include "protocol.h"

using namespace std;

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

    std::string request(std::move(recvStr(sockPtr)));
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
            if (fs.is_open()) {      //打开文件成功
                cout << "begin trans: " << requestBody.data << endl;

                int total = 0;
                int slice = 0;
                while (!fs.eof()) {              //读文件
                    fs.read(buf, buffer_size);
                    auto count = fs.gcount();
                    total += count;
                    size_t sendBytes = boost::asio::write(*sockPtr, buffer(buf, static_cast<size_t>(count)), boost::asio::transfer_exactly(
                            static_cast<size_t>(count)));
//                    boost::this_thread::sleep(boost::posix_time::milliseconds(1));
                    slice++;
                    cout << "slice: " << slice << " -> " << sendBytes << endl;
                }

//                cout << "total: " << total << endl;
//                //关闭文件
//                fs.close();
//
//                //关闭socket写一端
//                sockPtr->shutdown(boost::asio::socket_base::shutdown_type::shutdown_send);
//
                // 等待客户端确认接收完毕
                recvStr(sockPtr);
//
//                sockPtr->close();
                // 关闭socket
//                close(sockPtr);
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


std::string BoostTCPServerHelper::recvStr(ip::tcp::socket *sock) {
    cout << "beginRecv" << endl;
    boost::system::error_code ec;
    size_t bytes = sock->read_some(buffer(this->buf, this->buffer_size), ec);
    if(ec != nullptr)
        cerr << ec.message() << endl;
    while(ec == boost::asio::error::eof || bytes == 0) {
        bytes = sock->read_some(buffer(this->buf, this->buffer_size), ec);
    }
    cout << "收到: " << bytes << endl;
    return std::string(this->buf, bytes);
}

void BoostTCPServerHelper::writeStr(ip::tcp::socket *sock, const std::string &msg) {
    sock->write_some(buffer(msg));
}


void BoostTCPServerHelper::easyErr(ip::tcp::socket *sock, int code) {
    ResponseBody responseBody(code, mingj::protocol::error_code::codeToErrMsg(code));
    writeStr(sock, responseBody.toJson());
    close(sock);
}

void BoostTCPServerHelper::easySuccess(ip::tcp::socket *sock, const string &msg, int fileSize, int chunkSize) {
    ResponseBody responseBody(ProtocolHelper::RESPONSE_CODE_SUCCESS, msg, fileSize, chunkSize);
    writeStr(sock, responseBody.toJson());
}

void BoostTCPServerHelper::close(ip::tcp::socket *sockPtr) {
    if (nullptr != sockPtr) {
        sockPtr->close();
        delete sockPtr;
    }
}

