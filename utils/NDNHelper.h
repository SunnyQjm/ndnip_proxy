//
// Created by mingj on 19-2-28.
//

#ifndef NDNIP_PROXY_NDNHELPER_H
#define NDNIP_PROXY_NDNHELPER_H

#include <ndn-cxx/data.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include "JSONCPPHelper.h"

using namespace ndn;

typedef function<void(const Interest &, const Data&)> ON_DATA_CALLBACK;
typedef function<void(const Interest &, const lp::Nack &)> NACK_CALLBACK;
typedef function<void(const Interest &)> TIMEOUT_CALLBACK;
typedef const InterestCallback & INTEREST_CALLBACK;
typedef const RegisterPrefixFailureCallback & REGISTER_PREFIX_FAIL_CALLBACK;

class NDNHelper {
public:
    /**
     * 默认连接本地的NFD进程
     * @param connectUrl
     */
    explicit NDNHelper(const string &connectUrl = "127.0.0.1");

    /**
     * 发送兴趣包
     * @param name
     * @param onData
     * @param nack
     * @param timeout_callback
     */
    void expressInterest(const string &name, ON_DATA_CALLBACK onData, NACK_CALLBACK nack, TIMEOUT_CALLBACK timeout_callback);

    /**
     * 将数据放到缓存当中。如果有正在等待的数据包，就满足它
     * @param name
     * @param buf
     * @param size
     */
    void putData(const string &name, uint8_t * buf, size_t size);


    /**
     * 添加兴趣报过滤器
     */
    void addInterestFilter(const string &prefix, INTEREST_CALLBACK onInterest,
            REGISTER_PREFIX_FAIL_CALLBACK onRegisterFailed);

    /**
     * 开始处理NDN事件
     */
    void processEvent();

    void shutdown();

    /**
     * 开始异步处理NDN事件
     */
    void asyncProcessEvent();

private:
    Face face;
    KeyChain KeyChain_;

};


#endif //NDNIP_PROXY_NDNHELPER_H
