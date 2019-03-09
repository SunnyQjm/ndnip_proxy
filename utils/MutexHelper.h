//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_MUTEXHELPER_H
#define NDNIP_PROXY_MUTEXHELPER_H

#include <boost/thread/shared_mutex.hpp>

template <class T>
class MutexHelper {
public:
//    static void shareRead(boost::shared_mutex &mutex, std::function<void()> callback);
//    static void shareModify(boost::shared_mutex &mutex, std::function<void()> callback);
//
//    static bool shareModify(boost::shared_mutex &mutex, std::function<bool()> callback);
//    static bool shareRead(boost::shared_mutex &mutex, std::function<bool()> callback);

    static T shareModify(boost::shared_mutex &mutex, std::function<T()> callback);
    static T shareRead(boost::shared_mutex &mutex, std::function<T()> callback);

};

template<class T>
T MutexHelper<T>::shareModify(boost::shared_mutex &mutex, std::function<T()> callback) {
    boost::unique_lock<boost::shared_mutex> m(mutex);
    return callback();
}

template<class T>
T MutexHelper<T>::shareRead(boost::shared_mutex &mutex, std::function<T()> callback) {
    boost::shared_lock<boost::shared_mutex> m(mutex);
    return callback();
}


#endif //NDNIP_PROXY_MUTEXHELPER_H
