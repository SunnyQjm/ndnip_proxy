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
    try {
        this->close();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
}


void BoostTCPClientHelper::getFileFromServerAndBeginTrans(const std::string &fileName,
                                                          function<void(ResponseBody &)> onResponse,
                                                          function<void(uint8_t *, size_t, int)> callback) {
    cout << "getFileFromServerAndBeginTrans" << endl;
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE, fileName);

    // 发送传输文件请求
    sendStr(sock, requestBody.toJson());

    cout << "request: " << requestBody.toJson() << endl;

    string responseJson;
    this->asyncVisitBuf([=, &responseJson] {
        responseJson = readStr(sock, this->strBuf, this->buffer_size);
    }, this->strBufMutex);

    cout << "response: " << responseJson << endl;

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);

    // 服务器的消息
    onResponse(responseBody);

    int total = 0;
    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件

        // 发送给服务器，让服务器开始传文件
        sendStr(sock, "\n");
        int totalSize = responseBody.fileSize;
        int count = 0;
        while (totalSize > 0) {
            this->asyncVisitBuf([=, &count, &totalSize, &total] {
                size_t bytes = readn(sock, this->buf,
                                     totalSize > responseBody.chunkSize ? responseBody.chunkSize : totalSize);
                callback((uint8_t *) this->buf, bytes, count++);
                totalSize -= bytes;
                total += bytes;
            }, this->bufMutex);
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
    this->asyncVisitBuf([=, &responseJson] {
        responseJson = readStr(sock, this->strBuf, this->buffer_size);
    }, this->strBufMutex);

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);

    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件
        // 服务器的消息
        onResponse(responseBody);
        // 发送给服务器，让服务器开始传文件
        sendStr(sock, "\n");

        this->asyncVisitBuf([=] {
            size_t bytes = readn(sock, this->sliceBuf, responseBody.chunkSize);
            callback((uint8_t *) this->sliceBuf, bytes);
        }, this->sliceBufMutex);

        sock->close();
    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
}


void
BoostTCPClientHelper::getFileInfoFromServer(const std::string &fileName, function<void(ResponseBody &)> onResponse) {
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE_INFO, fileName);
    // 发送获取文件信息的请求
    sendStr(sock, requestBody.toJson());

    // 接收回复
    string responseJson;
    this->asyncVisitBuf([=, &responseJson] {
        responseJson = readStr(sock, this->strBuf, this->buffer_size);
    }, this->strBufMutex);
    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);
    onResponse(responseBody);
    sendStr(sock, "\n");
    close();
}

void BoostTCPClientHelper::testBlockChainRequest() {
    RequestBody requestBody(0, "wtf?");
    sendStr(sock, requestBody.toJson());
}

void BoostTCPClientHelper::asyncVisitBuf(function<void()> callback, boost::shared_mutex &mutex) {
    boost::shared_lock<boost::shared_mutex> m(mutex);
    callback();
}

void BoostTCPClientHelper::close() {
    if (sock == nullptr)
        return;
    try {
        if (sock->is_open()) {
            sock->close();
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    delete sock;
    sock = nullptr;
}




