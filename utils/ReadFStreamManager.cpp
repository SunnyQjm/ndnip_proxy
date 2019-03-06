//
// Created by mingj on 19-3-6.
//

#include <boost/thread/shared_mutex.hpp>
#include "ReadFStreamManager.h"

namespace mingj {
    namespace stream_util {
        ReadFStreamManager *ReadFStreamManager::getInstance() {
            static ReadFStreamManager *instance = nullptr;
            static boost::shared_mutex mutex;
            if (instance == nullptr) {
                boost::shared_lock<boost::shared_mutex> m(mutex);
                if (instance == nullptr) {
                    instance = new ReadFStreamManager();
                }
            }
            return instance;
        }

        ReadFStreamManager *ReadFStreamManager::updateBaseResourcePath(const std::string &path) {
            boost::filesystem::path tempPath = boost::filesystem::path(path);
            if (!boost::filesystem::exists(tempPath)) {
                throw FileOptError::FILE_NOT_EXISTS_ERROR();
            }
            this->basePath = boost::filesystem::path(path);
            return this;
        }

        ReadFStreamManager *ReadFStreamManager::updateBaseResourcePath(const boost::filesystem::path &path) {
            if (!boost::filesystem::exists(path)) {
                throw FileOptError::FILE_NOT_EXISTS_ERROR();
            }
            this->basePath = path;
            return this;
        }

        boost::filesystem::fstream *ReadFStreamManager::getReadString(const boost::filesystem::path &path) {
            if (!boost::filesystem::exists(path)) {
                throw FileOptError::FILE_NOT_EXISTS_ERROR();
            }
            if (!boost::filesystem::is_regular_file(path)) {
                throw FileOptError::NOT_A_REGULAR_FILE();
            }
            boost::filesystem::fstream *pfs = nullptr;
            {
                boost::shared_lock<boost::shared_mutex> m(this->mapVisitMutex);
                auto result = map.find(path.string());
                if (result == map.end()) {       //未找到则新打开一个
                    pfs = new boost::filesystem::fstream(path, std::ios_base::binary | std::ios_base::in);
                    map.insert(make_pair(path.string(), pfs));
                } else {                        //找到则取出
                    pfs = result->second;
                }
            }
            return pfs;
        }

        bool ReadFStreamManager::remove(const boost::filesystem::path &path) {
            boost::shared_lock<boost::shared_mutex> m(this->mapVisitMutex);
            auto result = map.find(path.string());
            if (result == map.end()) {       //未找到则新打开一个
                return false;
            } else {                        //找到则取出
                auto pfs = result->second;
                pfs->close();
                delete pfs;
                return true;
            }
        }

        ReadFStreamManager *ReadFStreamManager::clear() {
            for (auto &v : map) {
                auto pfs = v.second;
                pfs->close();
                delete pfs;
            }
            map.clear();
            return this;
        }
    }
}