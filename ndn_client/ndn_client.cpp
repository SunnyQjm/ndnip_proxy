//
// Created by mingj on 19-2-28.
//

//#include <FileUtils.h>
//#include "ndn_client.h"
//#include "protocol.h"
//#include "NDNHelper.h"
//#include <cmath>
//#include "OnlinePreviewer.h"
//#include <boost/thread.hpp>
//#include "ThreadPool.h"
#include "NdnClientHelper.h"

//long getCurTime() {
//    auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(
//            chrono::system_clock::now()
//                    .time_since_epoch());
//    return duration_in_ms.count();
//}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cout << "usage: ./nc <configFilePath> <ip> <port> <fileName>" << endl;
        return 0;
    }
    NdnClientHelper::getInstance()
            ->updateConfig(argv[1])
            ->getFile(argv[2], static_cast<unsigned short>(atoi(argv[3])), argv[4])
            ->start();
//    string base = "/IP/";
//    base += argv[1];
//    base += "/";
//    base += argv[2];
//    base += "/";
//
//    string fileName = argv[3];
//    NDNHelper ndnHelper;
//
//
//    auto outputPath = FileUtils::getOutputPath();
//    outputPath.append(fileName);
//    auto nack = [=](const Interest &interest, const lp::Nack &nack) {        //onNack
//        cout << "NACK" << endl;
//    };
//    auto timeout = [=](const Interest &interest) {          //timeout
//        cout << interest.getName().toUri() << " timeout" << endl;
//    };
//
//    boost::filesystem::fstream os;
//    bool preview = false;
////    ThreadPool pool(4);
//    std::function<void(const Interest &interest, const Data &data, int position,
//                       int totalCount)> getFileOnData;
//    long start = getCurTime();
//    auto getFileInfoOnData = [=, &ndnHelper, &os, &preview, &getFileOnData](const Interest &interest,
//                                                                            const Data &data) {    // onData
//        cout << "onData" << boost::this_thread::get_id() << endl;
//        const auto &content = data.getContent();
//        string json((char *) content.value(), content.value_size());
//        ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(json);
//        cout << responseBody.toJson() << endl;
//
//        //////////////////////////////////////////
//        /////// 下面获取所有的文件块，并写到文件当中
//        //////////////////////////////////////////
//
//        //如果不存在就创建文件
//        FileUtils::createFileIfNotExist(outputPath);
//
//        os.open(outputPath, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
//        int count = (int) (responseBody.fileSize / responseBody.chunkSize) + 1;
//
//
//        getFileOnData = [=, &os, &ndnHelper, &preview, &getFileOnData](const Interest &interest, const Data &data,
//                                                                       int position,
//                                                                       int totalCount) {
//            // 偏移到指定位置
//            os.seekp(position * responseBody.chunkSize, std::ios::beg);
//            os.write((char *) data.getContent().value(),
//                     data.getContent().value_size());
////            if (!preview && (position > 1000 || 2 * position > count)) {
////                preview = true;
////                os.close();
////                boost::thread t([=]() {
////                    OnlinePreviewer().preview(outputPath.string());
////                });
////                os.open(outputPath, std::ios_base::binary | std::ios_base::out |
////                                    std::ios_base::in);
////
////            }
//            if (position == count - 1) {
//                cout << "总共有： " << count << endl;
//                os.flush();
//                os.close();
//
//                long end = getCurTime();
//                cout << end - start << endl;
//                //手动退出
//                exit(0);
//            } else {
//                ndnHelper.expressInterest(base + fileName + "/" + to_string(position + 1), std::bind(getFileOnData,
//                                                                                                     std::placeholders::_1,
//                                                                                                     std::placeholders::_2,
//                                                                                                     position + 1,
//                                                                                                     totalCount),
//                                          nack, timeout);
//            }
//        };
//        ndnHelper.expressInterest(base + fileName + "/" + to_string(0), std::bind(getFileOnData,
//                                                                                  std::placeholders::_1,
//                                                                                  std::placeholders::_2,
//                                                                                  0, count),
//                                  nack, timeout);
//    };
//    ndnHelper.expressInterest(
//            base + fileName,
//            getFileInfoOnData, nack, timeout);
//    ndnHelper.processEvent();
    return 0;
}