//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_MUTEXHELPER_H
#define NDNIP_PROXY_MUTEXHELPER_H

#include <boost/thread/shared_mutex.hpp>

class MutexHelper {
public:
    static void shareRead(boost::shared_mutex &mutex, std::function<void()> callback);
    static void shareModify(boost::shared_mutex &mutex, std::function<void()> callback);
};


#endif //NDNIP_PROXY_MUTEXHELPER_H
