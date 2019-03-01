//
// Created by mingj on 19-2-27.
//

#ifndef NDNIP_PROXY_FILEUTILS_H
#define NDNIP_PROXY_FILEUTILS_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <string>

class FileUtils {
public:
    static boost::filesystem::path getCurrentPath();

    static boost::filesystem::path getResourcePath();

    static boost::filesystem::path getOutputPath();

    static std::string RES_PATH;
    static std::string OUTPUT_PATH;
};




#endif //NDNIP_PROXY_FILEUTILS_H
