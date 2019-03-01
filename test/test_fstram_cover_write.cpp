//
// Created by mingj on 19-3-1.
//

#include "FileUtils.h"
#include <iostream>
using namespace std;
int main() {
    auto outputPath = FileUtils::getCurrentPath();
    outputPath.append("test.txt");
    boost::filesystem::fstream os(outputPath, std::ios_base::out | std::ios_base::in);
    os.seekp(2, std::ios::beg);
    string test = "test";
    os.seekp(0, std::ios::beg);
    os.write("ab", 2);
    os.write(test.c_str(), test.size());
    os.close();
}