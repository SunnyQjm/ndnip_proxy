//
// Created by mingj on 19-3-8.
//

#include "MutexHelper.h"

//template<>
//void MutexHelper<void>::shareRead(boost::shared_mutex &mutex, std::function<void()> callback) {
//    boost::shared_lock<boost::shared_mutex> m(mutex);
//    callback();
//}
//
//template<>
//void MutexHelper<void>::shareModify(boost::shared_mutex &mutex, std::function<void()> callback) {
//    boost::unique_lock<boost::shared_mutex> m(mutex);
//    callback();
//}

//template<>
//bool MutexHelper<bool>::shareModify(boost::shared_mutex &mutex, std::function<bool()> callback) {
//    boost::unique_lock<boost::shared_mutex> m(mutex);
//    return callback();
//}
//
//template<>
//bool MutexHelper<bool>::shareRead(boost::shared_mutex &mutex, std::function<bool()> callback) {
//    boost::shared_lock<boost::shared_mutex> m(mutex);
//    return callback();
//}

