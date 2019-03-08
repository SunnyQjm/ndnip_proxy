//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_RESOURCEMANAGERFACTORY_H
#define NDNIP_PROXY_RESOURCEMANAGERFACTORY_H

#include <functional>
#include <iostream>

namespace mingj {
    namespace manager {
        template<class T>
        class ResourceManager {

        public:

            explicit ResourceManager(T &t, std::function<void(T &t)> closeCallback);

            explicit ResourceManager(T &t);

            ~ResourceManager();

        private:
            T &t;
            std::function<void(T &t)> closeCallback;
        };

        template<class T>
        ResourceManager<T>::ResourceManager(T &t, std::function<void(T &t)> closeCallback):
                t(t), closeCallback(closeCallback) {
        }

        template<class T>
        ResourceManager<T>::~ResourceManager() {
            try {
                closeCallback(t);
            } catch (std::exception &e) {       //吞掉异常
                std::cerr << e.what() << std::endl;
            }
        }

        template<class T>
        ResourceManager<T>::ResourceManager(T &t):t(t), closeCallback([]() {

        }) {
        }

    }
}


#endif //NDNIP_PROXY_RESOURCEMANAGERFACTORY_H
