//
// Created by mingj on 19-2-28.
//

#include <FileUtils.h>
#include "ndn_client.h"
#include "protocol.h"
#include "NDNHelper.h"
#include <cmath>
#include "OnlinePreviewer.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "usage: ./nc <ip> <port> <fileName>" << endl;
        return 0;
    }
    string base = "/IP/";
    base += argv[1];
    base += "/";
    base += argv[2];
    base += "/";
    string fileName = argv[3];
    NDNHelper ndnHelper;
    auto outputPath = FileUtils::getOutputPath();
    outputPath.append(fileName);
    auto nack = [=](const Interest &interest, const lp::Nack &nack) {        //onNack
        cout << "NACK" << endl;
    };
    auto timeout = [=](const Interest &interest) {          //timeout
        cout << interest.getName().toUri() << " timeout" << endl;
    };
    ndnHelper.expressInterest(
            base + fileName,
            [=, &ndnHelper](const Interest &interest, const Data &data) {    // onData
                cout << "onData" << endl;
                const auto &content = data.getContent();
                string json((char *) content.value(), content.value_size());
                ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(json);
                cout << responseBody.toJson() << endl;

                //////////////////////////////////////////
                /////// 下面获取所有的文件块，并写到文件当中
                //////////////////////////////////////////
                boost::filesystem::fstream os(outputPath, std::ios_base::binary | std::ios_base::out);
                int count = (int) (responseBody.fileSize / responseBody.chunkSize) + 1;

                // 开始获取所有的文件块
                for (int i = 0; i < count; i++) {
                    ndnHelper.expressInterest(base + fileName + "/" + to_string(i),
                                              [=, &os, &ndnHelper](const Interest &interest, const Data &data) {
                                                  // 偏移到指定位置
                                                  os.seekp(i * responseBody.chunkSize, std::ios::beg);
                                                  os.write((char *) data.getContent().value(),
                                                           data.getContent().value_size());
                                                  if (i == count - 1) {
                                                      cout << "总共有： " << count << endl;
                                                      os.flush();
                                                      os.close();

//                                                      OnlinePreviewer().preview(outputPath.string());
                                                      //手动退出
                                                      exit(0);
                                                  }
                                              }, nack, timeout);
                }
                ndnHelper.processEvent();
            }, nack, timeout);
    ndnHelper.processEvent();
}