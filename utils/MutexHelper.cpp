//
// Created by mingj on 19-3-8.
//

#include "MutexHelper.h"

void MutexHelper::shareRead(boost::shared_mutex &mutex, std::function<void()> callback) {
    boost::shared_lock<boost::shared_mutex> m(mutex);
    callback();
}

void MutexHelper::shareModify(boost::shared_mutex &mutex, std::function<void()> callback) {
    boost::unique_lock<boost::shared_mutex> m(mutex);
    callback();
}
