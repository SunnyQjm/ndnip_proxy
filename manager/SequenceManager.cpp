//
// Created by mingj on 19-3-8.
//

#include <boost/thread/shared_mutex.hpp>
#include "SequenceManager.h"

namespace mingj {
    namespace manager {
        SequenceManager *SequenceManager::getInstance() {
            static SequenceManager* instance = nullptr;
            if(instance == nullptr) {
                static boost::shared_mutex mutex;
                boost::shared_lock<boost::shared_mutex> m(mutex);
                if(instance == nullptr) {
                    instance = new SequenceManager();
                }
            }
            return nullptr;
        }
    }
}