//
// Created by mingj on 19-2-27.
//

#include "BoostTCPClientHelper.h"
#include "FileUtils.h"

BoostTCPClientHelper::BoostTCPClientHelper(const std::string &ip, unsigned short port, size_t buffer_size) :
        ep(ip::address_v4::from_string(ip), port), buffer_size(buffer_size) {

}

void BoostTCPClientHelper::connect() {

    // delete空指针不影响
    delete sock;
    sock = new ip::tcp::socket(service);

    sock->connect(ep);
}

BoostTCPClientHelper::~BoostTCPClientHelper() {
    delete sock;
}


void BoostTCPClientHelper::getFileFromServer(const std::string &fileName, function<void(ResponseBody &)> onResponse,
                                             function<void(uint8_t *, size_t, int)> callback) {
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE, fileName);

    // 发送传输文件请求
    sendStr(sock, requestBody.toJson());

    string responseJson = readStr(sock, this->buf, this->buffer_size);

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);

    // 服务器的消息
    onResponse(responseBody);
    cout << responseBody.toJson() << endl;
    int total = 0;
    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件

        // 发送给服务器，让服务器开始传文件
        sendStr(sock, "\n");
        int totalSize = responseBody.fileSize;
        int count = 0;
        while (totalSize > 0) {
            this->asyncVisitBuf([=, &count, &totalSize, &total] {
                boost::shared_lock<boost::shared_mutex> m(this->bufMutex);
                size_t bytes = readn(sock, this->buf,
                                     totalSize > responseBody.chunkSize ? responseBody.chunkSize : totalSize);
                callback((uint8_t *) this->buf, bytes, count++);
                totalSize -= bytes;
                total += bytes;
            });
        }
        sock->close();
    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
    cout << "total: " << total << endl;

}

void
BoostTCPClientHelper::getFileSliceFromServer(const std::string &fileName, int sliceNum,
                                             function<void(ResponseBody &)> onResponse,
                                             function<void(uint8_t *, size_t)> callback) {
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE_SLICE, fileName, sliceNum);

    cout << requestBody.toJson() << endl;

    // 发送传输文件请求
    sendStr(sock, requestBody.toJson());
    string responseJson;
//    this->asyncVisitBuf([=, &responseJson]{
    responseJson = readStr(sock, this->sliceBuf, this->buffer_size);
//    });

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);

    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件
        // 服务器的消息
        onResponse(responseBody);
        // 发送给服务器，让服务器开始传文件
        sendStr(sock, "\n");

//        this->asyncVisitBuf([=]{
        size_t bytes = readn(sock, this->sliceBuf, responseBody.chunkSize);
        callback((uint8_t *) this->sliceBuf, bytes);
//        });

        sock->close();
    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
}

void BoostTCPClientHelper::testBlockChainRequest() {
    RequestBody requestBody(0, "wtf?");
    sendStr(sock, requestBody.toJson());
}

void BoostTCPClientHelper::asyncVisitBuf(function<void()> callback) {
    boost::shared_lock<boost::shared_mutex> m(this->bufMutex);
    callback();
}



