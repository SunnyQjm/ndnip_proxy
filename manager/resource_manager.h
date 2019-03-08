//
// Created by mingj on 19-3-8.
//

#ifndef NDNIP_PROXY_RESOURCE_MANAGER_H
#define NDNIP_PROXY_RESOURCE_MANAGER_H

#include "ResourceManager.h"

/**
 * 包含资源管理对象模板的各种特化版本，如果需要使用，需要同时引入该特化版本关联的头文件
 */
namespace mingj {
    namespace manager {

# if defined(BOOST_FILESYSTEM3_FSTREAM_HPP)
        /**
         * 针对 boost::filesystem::fstream 的特化版本
         */
        template<>
        ResourceManager<boost::filesystem::fstream>::ResourceManager(boost::filesystem::fstream &t)
                :
                t(t), closeCallback([](boost::filesystem::fstream &fs) {
            fs.close();
        }) {
        }
#endif
    }
}
#endif //NDNIP_PROXY_RESOURCE_MANAGER_H
