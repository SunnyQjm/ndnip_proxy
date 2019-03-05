//
// Created by mingj on 19-3-5.
//

#include "NdnClientHelper.h"

const string NdnClientHelper::FILE_SLICE_PREFIX = "fileSlicePrefix";
const string NdnClientHelper::FILE_INFO_PREFIX = "fileInfoPrefix";
const string NdnClientHelper::IS_PREVIEW = "isPreview";

NdnClientHelper *NdnClientHelper::getInstance() {
    static NdnClientHelper *instance = nullptr;
    static boost::shared_mutex mutex;
    if (instance == nullptr) {
        boost::shared_lock<boost::shared_mutex> m(mutex);
        if (instance == nullptr) {
            instance = new NdnClientHelper();
        }
    }
    return instance;
}

NdnClientHelper *NdnClientHelper::updateConfig(const string &configPath) {
    JSONCPPHelper jsoncppHelper(configPath, 1);

    this->fileSlicePrefix = jsoncppHelper.getString(FILE_SLICE_PREFIX);
    vector<string> fileds;
    boost::split(fileds, this->fileSlicePrefix, boost::is_any_of("/"));
    this->fileSlicePrefixLen = fileds.size();


    this->fileInfoPrefix = jsoncppHelper.getString(FILE_INFO_PREFIX);
    fileds.clear();
    boost::split(fileds, this->fileInfoPrefix, boost::is_any_of("/"));
    this->fileInfoPrefixLen = fileds.size();

    this->isPreview = jsoncppHelper.getBool(IS_PREVIEW);
    return this;
}

NdnClientHelper *NdnClientHelper::getFile(const string &ip, unsigned short port, const string &fileName) {
    ndnHelper.expressInterest(
            this->getBaseFileInfoPrefix(ip, port, fileName) + "/" + UUIDUtils::generateUUid(),
            std::bind(&NdnClientHelper::getFileInfoOnData, this, std::placeholders::_1, std::placeholders::_2,
                      ip, port, fileName),
            std::bind(&NdnClientHelper::onNack, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&NdnClientHelper::onTimeout, this, std::placeholders::_1));
    return this;
}

void NdnClientHelper::onNack(const Interest &interest, const lp::Nack &nack) {
    cout << "NACK: " << interest.getName().toUri() << endl;
}

void NdnClientHelper::onTimeout(const Interest &interest) {
    cout << interest.getName().toUri() << " timeout" << endl;
}

void NdnClientHelper::getFileInfoOnData(const Interest &interest, const Data &data, const string &ip,
                                        unsigned short port, const string &fileName) {
    const auto &content = data.getContent();
    string json((char *) content.value(), content.value_size());
    ResponseBody responseBody = ProtocolHelper::jsonToResponseBody(json);
    cout << responseBody.toJson() << endl;

    //////////////////////////////////////////
    /////// 下面获取所有的文件块，并写到文件当中
    //////////////////////////////////////////

    auto outputPath = FileUtils::getOutputPath();

    //如果不存在就创建文件
    FileUtils::createFileIfNotExist(outputPath);

    this->os.open(outputPath, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
    int count = (int) (responseBody.fileSize / responseBody.chunkSize) + 1;
    string basePrefix = this->getBaseFileSlicePrefix(ip, port, fileName);
    ndnHelper.expressInterest(basePrefix + "/" + to_string(0),
                              std::bind(&NdnClientHelper::getFileOnData, this, std::placeholders::_1,
                                        std::placeholders::_2, 0, count, basePrefix, responseBody.chunkSize),
                              std::bind(&NdnClientHelper::onNack, this, std::placeholders::_1, std::placeholders::_2),
                              std::bind(&NdnClientHelper::onTimeout, this, std::placeholders::_1));
}

void NdnClientHelper::getFileOnData(const Interest &interest, const Data &data, int position, int totalCount,
                                    const string basePrefix, int chunkSize) {
    // 偏移到指定位置
    os.seekp(position * chunkSize, std::ios::beg);
    // 将数据写入到文件当中
    os.write((char *) data.getContent().value(),
             data.getContent().value_size());
//            if (!preview && (position > 1000 || 2 * position > count)) {
//                preview = true;
//                os.close();
//                boost::thread t([=]() {
//                    OnlinePreviewer().preview(outputPath.string());
//                });
//                os.open(outputPath, std::ios_base::binary | std::ios_base::out |
//                                    std::ios_base::in);
//
//            }
    if (position == totalCount - 1) {
        os.flush();
        os.close();

        //手动退出
        exit(0);
    } else {
        ndnHelper.expressInterest(basePrefix + "/" + to_string(position + 1),
                                  std::bind(&NdnClientHelper::getFileOnData, this, std::placeholders::_1,
                                            std::placeholders::_2, position + 1, totalCount, basePrefix, chunkSize),
                                  std::bind(&NdnClientHelper::onNack, this, std::placeholders::_1,
                                            std::placeholders::_2),
                                  std::bind(&NdnClientHelper::onTimeout, this, std::placeholders::_1));
    }
}

const string NdnClientHelper::getBaseFileInfoPrefix(const string &ip, unsigned short port, const string &fileName) {
    return this->fileInfoPrefix + "/" + ip + "/" + to_string(port) + "/" + fileName;
}

const string NdnClientHelper::getBaseFileSlicePrefix(const string &ip, unsigned short port, const string &fileName) {
    return this->fileSlicePrefix + "/" + ip + "/" + to_string(port) + "/" + fileName;
}

void NdnClientHelper::start() {
    ndnHelper.processEvent();
}


