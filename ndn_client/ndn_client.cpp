//
// Created by mingj on 19-2-28.
//

#include <FileUtils.h>
#include "ndn_client.h"
#include "protocol.h"
#include "NDNHelper.h"
#include <cmath>

int main() {
    string base = "/IP/127.0.0.1/9748/";
    string fileName = "ndn.mp4";
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
//                                                  cout << i << ": " << data.getContent().value_size() << endl;
                                                  os.write((char *) data.getContent().value(),
                                                           data.getContent().value_size());
                                                  if(i == count -1) {
                                                      cout << "总共有： " << count << endl;
                                                      os.flush();
                                                      os.close();
//                                                      ndnHelper.shutdown();
                                                  }
                                              }, nack, timeout);
                }
                ndnHelper.processEvent();
            }, nack, timeout);
    ndnHelper.processEvent();
}