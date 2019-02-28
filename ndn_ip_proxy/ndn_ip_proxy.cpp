//
// Created by mingj on 19-2-27.
//

#include "ndn_ip_proxy.h"
#include <boost/algorithm/string.hpp>
#include "NDNHelper.h"
#include "BoostTCPClientHelper.h"


/**
 * 格式 /IP/192.168.0.1/8080/ndn.mp4
 * @param filter
 * @param interest
 */
void onInterest(const InterestFilter &filter, const Interest &interest, NDNHelper *ndnHelper) {
    string interestName = interest.getName().toUri();
    cout << "onInterest: " << interestName << endl;

    vector<string> fileds;
    boost::split(fileds, interestName, boost::is_any_of("/"));
    if (fileds.size() != 5) {  //可能是格式不对
        cout << "fileds size" << fileds.size() << endl;
        return;
    }
    string ip = fileds[2];
    auto port = static_cast<unsigned short>(atoi(fileds[3].c_str()));
    string fileName = fileds[4];
    BoostTCPClientHelper boostTCPClientHelper(ip, port);
    boostTCPClientHelper.connect();
    string basePrefix = "/IP/" + ip + "/" + fileds[3] + "/" + fileName + "/";

    // 对拉取文件的请求，去目的IP主机拉取文件并放到缓存当中
    boostTCPClientHelper.getFileFromServer(fileName, [=, &ndnHelper](ResponseBody &responseBody) {   //请求文件返回结果
        string json = responseBody.toJson();
        ndnHelper->putData(interestName, (uint8_t *) json.c_str(), json.size());
    }, [=, &ndnHelper](uint8_t *buf, size_t bytes, int count) {                        //如果文件存在，则IP主机会返回文件流，传输的每一段文件，都会调用这个回调
        cout << "receive: " << count << endl;
        ndnHelper->putData(basePrefix + to_string(count), buf, bytes);
    });
}

void onRegisterFailed(const Name &prefix) {
    cout << "Register failed for prefix " << prefix.toUri() << endl;
}


int main(int argc, char *argv[]) {

    NDNHelper ndnHelper;
    ndnHelper.addInterestFilter("/IP",
                                (const InterestCallback &) std::bind(onInterest, std::placeholders::_1,
                                                                     std::placeholders::_2, &ndnHelper),
                                (const RegisterPrefixFailureCallback &) std::bind(onRegisterFailed,
                                                                                  std::placeholders::_1));
    ndnHelper.processEvent();

//    string fileName = "ndn.mp4";
//    auto outputPath = FileUtils::getOutputPath();
//    outputPath.append(fileName);
//    boost::filesystem::fstream os(outputPath, std::ios_base::binary | std::ios_base::out);
//    boostTCPClientHelper.getFileFromServer(fileName, [=, &os](char *buf, size_t bytes) {
//        os.write(buf, bytes);
//    });
//    os.close();
}
