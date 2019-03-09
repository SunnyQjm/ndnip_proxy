//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_SEQUENCEMANAGER_H
#define NDNIP_PROXY_SEQUENCEMANAGER_H

#include <boost/thread/shared_mutex.hpp>
#include <vector>

namespace mingj {
    namespace manager {

        class SlideWindow {
        public:
            explicit SlideWindow(size_t windowSize = 20);

            bool setValue(size_t position, bool value);

            bool getValue(size_t position);

            bool pop();

            /**
             * 进队列
             */
            bool push(bool value);

            /**
             * 判断是否为空
             * @return
             */
            bool empty();

            /**
             * 判断是否已满
             * @return
             */
            bool full();

            /**
             * 获取当前数据结构元素个数
             * @return
             */
            size_t size();

            /**
             * 查看队头元素
             * @return
             */
            bool front();

            /**
             * 查看队尾元素
             * @return
             */
            bool tail();

            void print();

        private:
            bool _empty();

            bool _full();

        private:
            size_t start;
            size_t end;
            size_t windowSize;
            size_t _size = 0;
            boost::shared_mutex mutex;
            std::vector<bool> window;
        };

        class SequenceManager {
        public:
            explicit SequenceManager(size_t firstSequence = 0, size_t windowSize = 20);

            /**
             * 获取下一个可用的序列号
             * @return
             */
            ssize_t getNextSequence();

            SlideWindow &getSlideWindow();

            /**
             * 确认一个序列号
             */
            void ackSequence(size_t sequence);

        private:
            size_t start;
            size_t end;
            size_t currentSequence;
            SlideWindow slideWindow;
            boost::shared_mutex mutex;
        };
    }
}


#endif //NDNIP_PROXY_SEQUENCEMANAGER_H
