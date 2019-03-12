//
// Created by mingj on 19-2-28.
//

//#include <boost/thread.hpp>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/name.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include "JSONCPPHelper.h"
#include "NDNHelper.h"
#include <iostream>
using namespace std;
NDNHelper::NDNHelper(const string &connectUrl) : face(connectUrl) {

}

void NDNHelper::expressInterest(const string &name, ON_DATA_CALLBACK onData, NACK_CALLBACK nack,
                                TIMEOUT_CALLBACK timeout_callback) {
    Interest interest(name);
    interest.setCanBePrefix(true);
    interest.setInterestLifetime(1_s);    //兴趣报存活时间
    this->face.expressInterest(interest, onData, nack, timeout_callback);
}

void NDNHelper::putData(const string &name, uint8_t *buf, size_t size) {
    Data data(name);
    data.setContent(buf, size);
    KeyChain_.sign(data);
    this->face.put(data);
    cout << "put data: " << name << endl;
}

void NDNHelper::addInterestFilter(const string &prefix, const InterestCallback &onInterest,
                                  const RegisterPrefixFailureCallback &onRegisterFailed) {
    this->face.setInterestFilter(prefix, onInterest, onRegisterFailed);
}

void NDNHelper::processEvent() {
    this->face.processEvents(time::milliseconds::zero(), true);
}

void NDNHelper::asyncProcessEvent() {

//    //开启一个线程处理事件
//    boost::thread t([=] {
//        this->face.processEvents();
//    });
}

void NDNHelper::shutdown() {
    this->face.shutdown();
}

