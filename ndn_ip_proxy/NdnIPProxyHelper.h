//
// Created by mingj on 19-3-5.
//

#ifndef NDNIP_PROXY_NDNIPPROXYHELPER_H
#define NDNIP_PROXY_NDNIPPROXYHELPER_H


#include <boost/algorithm/string.hpp>
#include "NDNHelper.h"
#include "BoostTCPClientHelper.h"
#include <boost/thread/pthread/shared_mutex.hpp>

using namespace std;
class NdnIPProxyHelper {
public:
    /**
     * 获取单例
     * @return
     */
    static NdnIPProxyHelper* getInstance();

    /**
     * 更新配置
     * @param configPath
     * @return
     */
    NdnIPProxyHelper* updateConfig(const string& configPath);

    NdnIPProxyHelper* stop();

    NdnIPProxyHelper* start();

    // Callback
    static void onRegisterFailed(const Name &prefix);

    static void onFileSliceInterest(const InterestFilter &filter, const Interest &interest, NDNHelper *ndnHelper);

    static void onFileInfoInterest(const InterestFilter &filter, const Interest &interest, NDNHelper *ndnHelper);


    const string &getFileSlicePrefix() const;

    size_t getFileSlicePrefixLen() const;

    const string &getFileInfoPrefix() const;

    size_t getFileInfoPrefixLen() const;

    const string getBaseFileSlicePrefix(const string& ip, unsigned short port, const string& fileName);

private:
    NDNHelper *pNdnHelper = nullptr;
    static const string FILE_SLICE_PREFIX;
    static const string FILE_INFO_PREFIX;
    string fileSlicePrefix;
    size_t fileSlicePrefixLen;
    string fileInfoPrefix;
    size_t fileInfoPrefixLen;
};


#endif //NDNIP_PROXY_NDNIPPROXYHELPER_H
