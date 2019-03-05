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

int FileUtils::createFileIfNotExist(const boost::filesystem::path &path, std::ios_base::openmode mode) {
    if(!boost::filesystem::exists(path)) {
        boost::filesystem::fstream out(path, mode);
        out.close();
    }
    return 0;
}

boost::filesystem::path FileUtils::getResourceFilePathIfExists(const std::string& fileName) {
    boost::filesystem::path filePath = FileUtils::getResourcePath();

    // 得到文件的绝对路径
    filePath.append(fileName);

    return filePath;
}

