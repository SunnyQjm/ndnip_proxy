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


void BoostTCPClientHelper::getFileFromServer(const std::string &fileName) {
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE, fileName);

    // 发送传输文件请求
    sendStr(sock, requestBody.toJson());

    string responseJson = readStr(sock, this->buf, this->buffer_size);

    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);
    cout << responseBody.toJson() << endl;
    int total = 0;
    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件

        // 发送给服务器，让服务器开始传文件
        sendStr(sock, "\n");

        auto outputPath = FileUtils::getOutputPath();
        outputPath.append(fileName);
        boost::filesystem::fstream os(outputPath, std::ios_base::binary | std::ios_base::out);
        int totalSize = responseBody.fileSize;
        while (totalSize > 0) {
            size_t bytes = readn(sock, this->buf,
                    totalSize > responseBody.chunkSize ? responseBody.chunkSize : totalSize);
            totalSize -= bytes;
            if (bytes == 0)
                break;
            os.write(this->buf, bytes);
            total += bytes;
        }

        os.close();

        sock->close();
    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
    cout << "total: " << total << endl;

}

