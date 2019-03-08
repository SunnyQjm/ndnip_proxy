//
// Created by mingj on 19-3-5.
//

#include "NdnIPProxyHelper.h"
#include "JSONCPPHelper.h"


const string NdnIPProxyHelper::FILE_SLICE_PREFIX = "fileSlicePrefix";
const string NdnIPProxyHelper::FILE_INFO_PREFIX = "fileInfoPrefix";

NdnIPProxyHelper *NdnIPProxyHelper::getInstance() {
    static NdnIPProxyHelper *instance = nullptr;
    if (instance == nullptr) {
        static boost::shared_mutex mutex;
        boost::shared_lock<boost::shared_mutex> m(mutex);
        if (instance == nullptr) {
            instance = new NdnIPProxyHelper();
        }
    }
    return instance;
}


NdnIPProxyHelper *NdnIPProxyHelper::updateConfig(const string &configPath) {
    JSONCPPHelper jsoncppHelper(configPath, 1);

    this->fileSlicePrefix = jsoncppHelper.getString(FILE_SLICE_PREFIX);
    vector<string> fileds;
    boost::split(fileds, this->fileSlicePrefix, boost::is_any_of("/"));
    this->fileSlicePrefixLen = fileds.size();


    this->fileInfoPrefix = jsoncppHelper.getString(FILE_INFO_PREFIX);
    fileds.clear();
    boost::split(fileds, this->fileInfoPrefix, boost::is_any_of("/"));
    this->fileInfoPrefixLen = fileds.size();
    return this;
}

NdnIPProxyHelper *NdnIPProxyHelper::start() {
    stop();
    this->pNdnHelper = new NDNHelper();

    cout << "begin listen: " << endl;
    cout << this->fileSlicePrefix << ": " << this->fileSlicePrefixLen << endl;
    cout << this->fileInfoPrefix << ": " << this->fileInfoPrefixLen << endl;
    try {
        this->pNdnHelper->addInterestFilter(this->fileSlicePrefix,
                                            (const InterestCallback &) std::bind(NdnIPProxyHelper::onFileSliceInterest,
                                                                                 std::placeholders::_1,
                                                                                 std::placeholders::_2, pNdnHelper),
                                            (const RegisterPrefixFailureCallback &) std::bind(
                                                    NdnIPProxyHelper::onRegisterFailed,
                                                    std::placeholders::_1));
        this->pNdnHelper->addInterestFilter(this->fileInfoPrefix,
                                            (const InterestCallback &) std::bind(NdnIPProxyHelper::onFileInfoInterest,
                                                                                 std::placeholders::_1,
                                                                                 std::placeholders::_2, pNdnHelper),
                                            (const RegisterPrefixFailureCallback &) std::bind(
                                                    NdnIPProxyHelper::onRegisterFailed,
                                                    std::placeholders::_1));
    } catch (std::exception &e) {
        cerr << e.what() << endl;
    }

    pNdnHelper->processEvent();
    return this;
}

NdnIPProxyHelper *NdnIPProxyHelper::stop() {
    if (pNdnHelper != nullptr) {
        delete pNdnHelper;
        pNdnHelper = nullptr;
    }
    return this;
}


void NdnIPProxyHelper::onRegisterFailed(const Name &prefix) {
    cerr << "Register failed for prefix " << prefix.toUri() << endl;
}

void
NdnIPProxyHelper::onFileSliceInterest(const InterestFilter &filter, const Interest &interest, NDNHelper *ndnHelper) {
    string interestName = interest.getName().toUri();
    cout << "onInterest: " << interestName << endl;

    vector<string> fileds;
    boost::split(fileds, interestName, boost::is_any_of("/"));

    size_t prefixLen = NdnIPProxyHelper::getInstance()->getFileSlicePrefixLen();
    if (fileds.size() != (prefixLen + 4)) {
        return;
    }
    string ip = fileds[prefixLen];
    auto port = static_cast<unsigned short>(atoi(fileds[prefixLen + 1].c_str()));

    string fileName = fileds[prefixLen + 2];
    int sliceNum = atoi(fileds[prefixLen + 3].c_str());
    boost::thread t([=]() {
        BoostTCPClientHelper boostTCPClientHelper(ip, port);
        boostTCPClientHelper.connect();
        boostTCPClientHelper.getFileSliceFromServer(fileName, sliceNum,
                                                    [=, &ndnHelper](ResponseBody &responseBody) {},
                                                    [=, &ndnHelper](uint8_t *buf,
                                                                    size_t bytes) {                        //如果文件存在，则IP主机会返回文件流，传输的每一段文件，都会调用这个回调
                                                        ndnHelper->putData(interestName, buf, bytes);
                                                    });
    });
}

void
NdnIPProxyHelper::onFileInfoInterest(const InterestFilter &filter, const Interest &interest, NDNHelper *ndnHelper) {
    string interestName = interest.getName().toUri();
    cout << "onInterest: " << interestName << endl;

    vector<string> fileds;
    boost::split(fileds, interestName, boost::is_any_of("/"));

    size_t prefixLen = NdnIPProxyHelper::getInstance()->getFileInfoPrefixLen();
    if (fileds.size() != (prefixLen + 4)) {
        return;
    }
    string ip = fileds[prefixLen];
    auto port = static_cast<unsigned short>(atoi(fileds[prefixLen + 1].c_str()));

    string fileName = fileds[prefixLen + 2];
    int challenge = atoi(fileds[prefixLen + 3].c_str());

    boost::thread t([=]() {
        BoostTCPClientHelper boostTCPClientHelper(ip, port);
        boostTCPClientHelper.connect();
        string basePrefix = NdnIPProxyHelper::getInstance()->getBaseFileSlicePrefix(ip, port, fileName);
        // 对拉取文件的请求，去目的IP主机拉取文件并放到缓存当中
        boostTCPClientHelper.getFileInfoFromServer(fileName, [=, &ndnHelper](
                ResponseBody &responseBody) {   //请求文件返回结果
            string json = responseBody.toJson();
            ndnHelper->putData(interestName, (uint8_t *) json.c_str(), json.size());
        });
//        boostTCPClientHelper.getFileFromServerAndBeginTrans(fileName, [=, &ndnHelper](
//                ResponseBody &responseBody) {   //请求文件返回结果
//            string json = responseBody.toJson();
//            ndnHelper->putData(interestName, (uint8_t *) json.c_str(), json.size());
//        }, [=, &ndnHelper](uint8_t *buf, size_t bytes,
//                           int count) {        //如果文件存在，则IP主机会返回文件流，传输的每一段文件，都会调用这个回调
//            ndnHelper->putData(basePrefix + to_string(count), buf, bytes);
//        });
    });
}

const string &NdnIPProxyHelper::getFileSlicePrefix() const {
    return fileSlicePrefix;
}

size_t NdnIPProxyHelper::getFileSlicePrefixLen() const {
    return fileSlicePrefixLen;
}

const string &NdnIPProxyHelper::getFileInfoPrefix() const {
    return fileInfoPrefix;
}

size_t NdnIPProxyHelper::getFileInfoPrefixLen() const {
    return fileInfoPrefixLen;
}

const string NdnIPProxyHelper::getBaseFileSlicePrefix(const string &ip, unsigned short port, const string &fileName) {
    return this->fileSlicePrefix + "/" + ip + "/" + to_string(port) + "/" + fileName + "/";
}

