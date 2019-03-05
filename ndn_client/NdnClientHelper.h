//
// Created by mingj on 19-3-5.
//

#ifndef NDNIP_PROXY_NDNCLIENTHELPER_H
#define NDNIP_PROXY_NDNCLIENTHELPER_H

#include "NDNHelper.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;

class NdnClientHelper {

public:
    static NdnClientHelper *getInstance();

    /**
     * 更新配置信息
     * @param configPath
     * @return
     */
    NdnClientHelper *updateConfig(const string &configPath);


    /**
     * 从一个IP主机获取文件
     * @return
     */
    NdnClientHelper *getFile(const string &ip, unsigned short port, const string &fileName);

    void start();

    ////////////////////////////////////////////
    /////// Callback
    ////////////////////////////////////////////
    void onNack(const Interest &interest, const lp::Nack &nack);

    void onTimeout(const Interest &interest);

    void getFileInfoOnData(const Interest &interest,
                           const Data &data, const string &ip, unsigned short port, const string &fileName);

    void getFileOnData(const Interest &interest, const Data &data,
                       int position,
                       int totalCount,
                       const string basePrefix, int chunkSize);

    const string getBaseFileInfoPrefix(const string &ip, unsigned short port, const string &fileName);

    const string getBaseFileSlicePrefix(const string &ip, unsigned short port, const string &fileName);

private:
    static const string FILE_SLICE_PREFIX;
    static const string FILE_INFO_PREFIX;
    static const string IS_PREVIEW;
    NDNHelper ndnHelper;
    boost::filesystem::fstream os;

    //config
    string fileSlicePrefix;
    size_t fileSlicePrefixLen;
    string fileInfoPrefix;
    size_t fileInfoPrefixLen;
    bool isPreview;
};


#endif //NDNIP_PROXY_NDNCLIENTHELPER_H
