//
// Created by mingj on 19-2-27.
//

#include "BoostTCPClientHelper.h"
#include "FileUtils.h"


BoostTCPClientHelper::BoostTCPClientHelper(const std::string &ip, in_port_t port, int buffer_size) {
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    // 初始化服务器地址信息
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr);
}

void BoostTCPClientHelper::connect() {
    int res = Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    cout << res << endl;
}

BoostTCPClientHelper::~BoostTCPClientHelper() = default;


void BoostTCPClientHelper::getFileFromServer(const std::string fileName) {
    cout << fileName << endl;
    RequestBody requestBody(ProtocolHelper::REQUEST_CODE_FILE, fileName);

    // 发送传输文件请求
    sendString(sockfd, requestBody.toJson());


    string responseJson = recvString(sockfd, this->buf, this->buffer_size);



    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(responseJson);
    cout << responseBody.toJson() << endl;
    int total = 0;


    if (responseBody.code == ProtocolHelper::RESPONSE_CODE_SUCCESS) { // 请求成功，且文件存在，则开始接收文件
        sendString(sockfd, "111");
        auto outputPath = FileUtils::getOutputPath();
        try {
            outputPath.append(fileName);
            boost::filesystem::fstream os(outputPath, std::ios_base::binary | std::ios_base::out);
            int totalSize = responseBody.fileSize;
            int count = 0;
            while (totalSize > 0) {
                ssize_t bytes = Readn(sockfd, this->buf,
                                      totalSize > responseBody.chunkSize ? responseBody.chunkSize : totalSize);
                if (bytes <= 0)
                    break;
                totalSize -= bytes;
                os.write(this->buf, bytes);
                count++;
                total += bytes;
                cout << "slice: " << count << " -> " << bytes << " -> " << totalSize << endl;
            }
            os.flush();
            os.close();

            //随意发送一个数据给服务器，通知它结束
            cout << "totalSize: " << totalSize << endl;
            sendString(sockfd, "111");
            Shutdown(sockfd, SHUT_RDWR);
        } catch (exception &e) {
            cout << outputPath.string() << endl;
//            outputPath.append(fileName);
//            cout << outputPath.string() << endl;
            cerr << e.what() << endl;
        }

    } else {
        cerr << "请求失败： " << responseBody.data << endl;
    }
    cout << "total: " << total << endl;

}

