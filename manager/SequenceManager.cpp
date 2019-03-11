//
// Created by mingj on 19-3-8.
//

#include "SequenceManager.h"
#include "MutexHelper.h"
#include <functional>
#include <iostream>

namespace mingj {
    namespace manager {
//        SequenceManager *SequenceManager::getInstance() {
//            static SequenceManager* instance = nullptr;
//            if(instance == nullptr) {
//                static boost::shared_mutex mutex;
//                boost::shared_lock<boost::shared_mutex> m(mutex);
//                if(instance == nullptr) {
//                    instance = new SequenceManager();
//                }
//            }
//            return nullptr;
//        }


        SlideWindow::SlideWindow(size_t windowSize)
                : windowSize(windowSize),
                  window(windowSize + 1, false),
                  start(0),
                  end(0) {

        }

        bool SlideWindow::_empty() {
            return start == end;
        }

        bool SlideWindow::empty() {
            boost::shared_lock<boost::shared_mutex> m(this->mutex);
            return _empty();
        }

        bool SlideWindow::_full() {
            return (end - start + windowSize + 1) % (windowSize + 1) == 1;
        }

        bool SlideWindow::full() {
            boost::shared_lock<boost::shared_mutex> m(this->mutex);
            return _full();
        }

        bool SlideWindow::push(bool value) {
            return MutexHelper<bool>::shareModify(this->mutex, [=] {
                // 满了则不插入
                if (this->_full()) {
                    return false;
                }
                size_t position = (start + 1) % (windowSize + 1);
                this->window[position] = value;
                this->start = position;
                this->_size++;
                return true;
            });
        }

        bool SlideWindow::pop() {
            return MutexHelper<bool>::shareModify(this->mutex, [=] {
                // 空则弹出失败
                if (this->_empty()) {
                    return false;
                }
                this->end = (this->end + 1) % (windowSize + 1);
                this->_size--;
                return true;
            });
        }

        size_t SlideWindow::size() {
            return MutexHelper<size_t>::shareRead(this->mutex, [=] {
                return this->_size;
            });
        }

        void SlideWindow::print() {
            std::cout << "size: " << size() << std::endl;
            std::cout << "start: " << start << std::endl;
            std::cout << "end: " << end << std::endl;
            std::cout << "is_full" << full() << std::endl;
            std::cout << "is_empty" << empty() << std::endl;
            for (auto v: window) {
                std::cout << ' ' << v << ' ' << std::endl;
            }
        }

        bool SlideWindow::setValue(size_t position, bool value) {
            return MutexHelper<bool>::shareModify(this->mutex, [=] {
                if (position < _size) {
                    auto realPosition = (end + 1 + position + windowSize + 1) % (windowSize + 1);
                    window[realPosition] = value;
                    return true;
                }
                return false;
            });
        }

        bool SlideWindow::getValue(size_t position) {
            return MutexHelper<bool>::shareRead(this->mutex, [=] {
                if (position < _size) {
                    auto realPosition = (end + 1 + position + windowSize + 1) % (windowSize + 1);
                    return window[realPosition];
                } else {
                    throw std::out_of_range("数组越界");
                }
            });
        }

        bool SlideWindow::front() {
            return MutexHelper<bool>::shareRead(this->mutex, [=] {
                if(!_empty()) {
                    return window[start];
                } else {
                    throw std::out_of_range("数组越界");
                }
            });
        }

        bool SlideWindow::tail() {
            return MutexHelper<bool>::shareRead(this->mutex, [=] {
                if(!_empty()) {
                    auto realPosition = (end + 1 + windowSize + 1) % (windowSize + 1);
                    return window[realPosition];
                } else {
                    throw std::out_of_range("数组越界");
                }
            });
        }


        /***************************************************
         *
         * Sequence Manager
         *
         ***************************************************/


        SequenceManager::SequenceManager(size_t firstSequence, size_t windowSize)
                : start(firstSequence),
                  slideWindow(windowSize),
                  end(firstSequence + windowSize),
                  currentSequence(firstSequence) {

        }

        ssize_t SequenceManager::getNextSequence() {
            return MutexHelper<ssize_t>::shareModify(this->mutex, [=]() {
                if (!slideWindow.full() && this->currentSequence < this->end) {         //可分配则返回分配的序列号
                    slideWindow.push(false);
                    return static_cast<ssize_t>(this->currentSequence++);
                }
                // 序列号超出
                return ssize_t(-1);
            });
        }

        void SequenceManager::ackSequence(size_t sequence) {
            MutexHelper<void>::shareModify(this->mutex, [=]() {
                slideWindow.setValue(sequence - start, true);

                // 窗口右移
                while(!slideWindow.empty() && slideWindow.tail()) {
                    slideWindow.pop();
                    start++;
                    end++;
                }
            });
        }

         SlideWindow &SequenceManager::getSlideWindow() {
            return slideWindow;
        }
    }
}