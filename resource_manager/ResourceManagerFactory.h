//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_RESOURCEMANAGERFACTORY_H
#define NDNIP_PROXY_RESOURCEMANAGERFACTORY_H

#include <functional>
#include <iostream>

namespace mingj {
    namespace factory {
        template<class T>
        class ResourceManagerFactory {

        public:

            explicit ResourceManagerFactory(T &t, std::function<void(T &t)> closeCallback);
            explicit ResourceManagerFactory(T &t);

            ~ResourceManagerFactory();

        private:
            T &t;
            std::function<void(T &t)> closeCallback;
        };

        template<class T>
        ResourceManagerFactory<T>::ResourceManagerFactory(T &t, std::function<void(T &t)> closeCallback):
                t(t), closeCallback(closeCallback) {
        }

        template<class T>
        ResourceManagerFactory<T>::~ResourceManagerFactory() {
            std::cout << "析构函数" << std::endl;
            closeCallback(t);
        }

        template<class T>
        ResourceManagerFactory<T>::ResourceManagerFactory(T &t):t(t), closeCallback([](){

        }) {
        }





    }
}


#endif //NDNIP_PROXY_RESOURCEMANAGERFACTORY_H
