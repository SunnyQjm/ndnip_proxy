//
// Created by mingj on 19-3-6.
//

#ifndef NDNIP_PROXY_READFSTREAMMANAGER_H
#define NDNIP_PROXY_READFSTREAMMANAGER_H

#include <unordered_map>
#include <boost/filesystem.hpp>
using namespace std;

namespace mingj {
    namespace stream_util {
        class FileOptError : public std::runtime_error {
        public:
            explicit FileOptError(const std::string &errMsg) : runtime_error(errMsg) {}

            static FileOptError FILE_NOT_EXISTS_ERROR() {
                return FileOptError("file not exists!!");
            }

            static FileOptError NOT_A_REGULAR_FILE() {
                return FileOptError("not a regular file!!");
            }
        };

        class ReadFStreamManager {
        public:
            ReadFStreamManager *getInstance();

            /**
             * 更新默认的基本路径
             * @param path
             */
            ReadFStreamManager * updateBaseResourcePath(const std::string &path);

            ReadFStreamManager * updateBaseResourcePath(const boost::filesystem::path &path);


            /**
             * 获取一个文件的只读的输入流
             * @param path
             * @return
             */
            boost::filesystem::fstream *getReadString(const boost::filesystem::path &path);

            /**
             * 移除对一个文件输入流的指针记录，并释放资源
             * @param path
             * @return
             */
            bool remove(const boost::filesystem::path &path);

            /**
             * 移除所有的记录并释放资源
             * @return
             */
            ReadFStreamManager * clear();
        private:
            unordered_map<std::string, boost::filesystem::fstream*> map;
            boost::shared_mutex mapVisitMutex;
            boost::filesystem::path basePath;
        };
    }
}

#endif //NDNIP_PROXY_READFSTREAMMANAGER_H
