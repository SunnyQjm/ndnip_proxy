//
// Created by mingj on 19-2-27.
//

#include "BoostTCPClientHelper.h"
#include "protocol.h"
#include "FileUtils.h"

BoostTCPClientHelper::BoostTCPClientHelper(const std::string &ip, size_t port, int buffer_size) {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

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

size_t BoostTCPClientHelper::send(char *buf, unsigned int len) {
    if (sock != nullptr) {
        return sock->write_some(buffer(buf, len));
    }
    return 0;
}


size_t BoostTCPClientHelper::recv(char *buf, unsigned int len) {
    boost::system::error_code ec;
    cout << "begin read" << endl;
//    size_t bytes = read(*sock, buffer(buf, len), ec);
    size_t bytes = sock->read_some(buffer(buf, len), ec);
    cout << "end read" << endl;
    if(ec != nullptr) {
        cerr << ec.message() << endl;
    }
    if(!ec || ec == boost::asio::error::eof) {
        return bytes;
    }

    return 0;
}

size_t BoostTCPClientHelper::send(const std::string &str) {
    return sock->write_some(buffer(str));
}

void BoostTCPClientHelper::close() {
    if (nullptr != sock) {
        sock->close();
    }
}



void BoostTCPClientHelper::getFileFromServer(const std::string &fileName) {
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE, fileName);

    // 发送传输文件请求
    send(requestBody.toJson());

    string responseJson = recvStr();

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);
    cout << responseBody.toJson() << endl;
    int total = 0;
    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件
        auto outputPath = FileUtils::getOutputPath();
        outputPath.append(fileName);
        boost::filesystem::fstream os(outputPath, std::ios_base::binary | std::ios_base::out);
        int totalSize = responseBody.fileSize;
        int count = 0;
        while (totalSize > 0) {
            size_t bytes = recv(this->buf, totalSize > responseBody.chunkSize ? responseBody.chunkSize : totalSize);
            totalSize -= bytes;
            if(bytes == 0)
                break;
            os.write(this->buf, bytes);
            count++;
            total += bytes;
            cout << "slice: " << count << " -> " << bytes << " -> " << totalSize<< endl;
        }
        os.close();

        //随意发送一个数据给服务器，通知它结束
        cout << "totalSize: " << totalSize << endl;
        send("11");
        sock->shutdown(boost::asio::socket_base::shutdown_type::shutdown_send);
    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
    cout << "total: " << total << endl;

}

std::string BoostTCPClientHelper::recvStr() {
    if (nullptr == sock) {
        return nullptr;
    }
    size_t bytes = sock->read_some(buffer(this->buf, this->buffer_size));
    return std::string(this->buf, bytes);
}

