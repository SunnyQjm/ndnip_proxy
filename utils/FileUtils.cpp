//
// Created by mingj on 19-2-27.
//

#include "FileUtils.h"

std::string FileUtils::RES_PATH = "resource";
std::string FileUtils::OUTPUT_PATH = "out";


boost::filesystem::path FileUtils::getCurrentPath() {
    return boost::filesystem::current_path();
}

boost::filesystem::path FileUtils::getResourcePath() {
    boost::filesystem::path path = getCurrentPath();
    path.append(RES_PATH);
    if(!boost::filesystem::exists(path)) {  //不存在该目录就创建该目录
        boost::filesystem::create_directories(path);
    }
//    std::cout << path.string() << std::endl;
    return path;
}

boost::filesystem::path FileUtils::getOutputPath() {
    boost::filesystem::path path = getCurrentPath();
    path.append(OUTPUT_PATH);
    if(!boost::filesystem::exists(path)) {  //不存在该目录就创建该目录
        boost::filesystem::create_directories(path);
    }
//    std::cout << path.string() << std::endl;
    return path;
}

