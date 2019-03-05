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

    static boost::filesystem::path getResourceFilePathIfExists(const std::string& fileName);

    static int createFileIfNotExist(const boost::filesystem::path &path,
                                    std::ios_base::openmode mode = std::ios::binary | std::ios::out);
    static std::string RES_PATH;
    static std::string OUTPUT_PATH;
};




#endif //NDNIP_PROXY_FILEUTILS_H
