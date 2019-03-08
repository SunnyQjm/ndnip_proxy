//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_SEQUENCEMANAGER_H
#define NDNIP_PROXY_SEQUENCEMANAGER_H

namespace mingj {
    namespace manager {
        class SequenceManager {
        public:
            static SequenceManager* getInstance();

        private:
            // 构造函数私有，不允许构造
            SequenceManager() = default;;

            int windowSize = 0;         //窗口大小


        };
    }
}


#endif //NDNIP_PROXY_SEQUENCEMANAGER_H
