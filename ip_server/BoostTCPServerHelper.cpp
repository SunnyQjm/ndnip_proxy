//
// Created by mingj on 19-2-27.
//
#include "BoostTCPServerHelper.h"
#include "FileUtils.h"
#include "MutexHelper.h"
#include "resource_manager.h"

using namespace mingj::manager;

BoostTCPServerHelper::BoostTCPServerHelper(unsigned short port, unsigned int buffer_size) :
        port(port),
        buffer_size(buffer_size) {

}

void BoostTCPServerHelper::startListen() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), port));
    while (true) {
        auto *sockPtr = new ip::tcp::socket(service);
        acceptor.accept(*sockPtr);
        boost::thread thread([=]() {
            PtrResourceManager<ip::tcp::socket> prm(sockPtr);
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
    std::string request;
    MutexHelper<void>::shareModify(this->strBufMutex, [=, &request] {
        request = readStr(sockPtr, this->strBuf, this->buffer_size);
    });

    RequestBody requestBody = ProtocolHelper::jsonToRequestBody(request);

    cout << request << endl;
    if (requestBody.code == ProtocolHelper::REQUEST_CODE_FILE
        || requestBody.code == ProtocolHelper::REQUEST_CODE_FILE_SLICE
        || requestBody.code == ProtocolHelper::REQUEST_CODE_FILE_INFO) {     //处理文件传输

        boost::filesystem::path filePath = FileUtils::getResourceFilePathIfExists(requestBody.data);

        // 判断文件是否存在，且是一个文件而不是目录
        if (boost::filesystem::exists(filePath)) { // 存在则进行文件传输操作

            // 首先告知客户端，文件存在。准备进行文件传输
            auto fileSize = boost::filesystem::file_size(filePath);
            size_t chunkSize = this->buffer_size;

            if (requestBody.code == ProtocolHelper::REQUEST_CODE_FILE_INFO) {
                easySuccess(sockPtr, "success", fileSize, chunkSize);
                return;
            }

            if(requestBody.code == ProtocolHelper::REQUEST_CODE_FILE_SLICE &&
                    requestBody.sliceNum * chunkSize > fileSize) {
                easyErr(sockPtr, mingj::protocol::error_code::ERR_READ_END_OF_FILE);
                return;
            }

            // 将fs交给资源管理对象，在资源管理对象的析构函数中会自动执行close

            boost::filesystem::fstream fs(filePath, std::ios_base::binary | std::ios_base::in);
            {
                ResourceManager<boost::filesystem::fstream> rmf(fs);         //将fs放进资源管理对象中，自动关闭
                if (fs.is_open()) {      //打开文件成功
                    try {
                        // 处理传输整个文件
                        if (requestBody.code == ProtocolHelper::REQUEST_CODE_FILE) {
                            easySuccess(sockPtr, "success", static_cast<int>(fileSize), chunkSize);

                            // 等待客户端写一行数据，标示可以开始传输文件
                            MutexHelper<void>::shareModify(this->strBufMutex, [=] {
                                readLine(sockPtr, this->strBuf, this->buffer_size);
                            });

                            int total = 0;
                            while (!fs.eof()) {              //读文件
                                MutexHelper<void>::shareModify(this->bufMutex, [=, &fs, &total] {
                                    fs.read(buf, chunkSize);
                                    auto count = fs.gcount();
                                    size_t sendBytes = writen(sockPtr, this->buf, static_cast<size_t>(count));
                                    total += sendBytes;
                                });
                            }

                            cout << "total: " << total << endl;
                        } else {    //处理传输一个文件块
                            size_t sliceSize = min(fileSize - (requestBody.sliceNum * chunkSize), chunkSize);
                            easySuccess(sockPtr, "success", static_cast<int>(fileSize),
                                        static_cast<unsigned int>(sliceSize));
                            // 等待客户端写一行数据，标示可以开始传输文件
                            MutexHelper<void>::shareModify(this->strBufMutex, [=] {
                                readLine(sockPtr, this->strBuf, this->buffer_size);
                            });

                            fs.seekg(chunkSize * requestBody.sliceNum, std::ios_base::beg);
                            MutexHelper<void>::shareModify(this->sliceBufMutex, [=, &fs] {
                                fs.read(this->sliceBuf, chunkSize);
                                auto count = fs.gcount();
                                writen(sockPtr, this->sliceBuf, static_cast<size_t>(count));
                            });
                        }
                    } catch (std::exception &e) {
                        cerr << e.what() << endl;
                    }

                    //关闭socket写一端（这样会将发送FIN给对端设备，flush发送缓存）
                    sockPtr->shutdown(boost::asio::socket_base::shutdown_type::shutdown_send);

                } else {
                    cout << "file not exists: " << filePath.string() << endl;
                    easyErr(sockPtr, mingj::protocol::error_code::ERR_FILE_COULD_NOT_OPEN);
                }
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
}

void BoostTCPServerHelper::easySuccess(ip::tcp::socket *sock, const string &msg, size_t fileSize,
                                       size_t chunkSize) {
    ResponseBody responseBody(ProtocolHelper::RESPONSE_CODE_SUCCESS, msg,
                              static_cast<unsigned int>(fileSize),
                              static_cast<unsigned int>(chunkSize));
    sendStr(sock, responseBody.toJson());
}


