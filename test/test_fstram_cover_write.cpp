//
// Created by mingj on 19-3-1.
//
//
//#include "FileUtils.h"
//#include <iostream>
//#include "BoostTCPClientHelper.h"
//
//using namespace std;
//
//void testFile() {
//    auto outputPath = FileUtils::getCurrentPath();
//    outputPath.append("test.txt");
//
//    FileUtils::createFileIfNotExist(outputPath);
//
//    boost::filesystem::fstream os(outputPath, std::ios_base::out | std::ios_base::in);
//    os.seekp(2, std::ios::beg);
//    string test = "test";
//    os.seekp(0, std::ios::beg);
//    os.write("ab", 2);
//    os.write(test.c_str(), test.size());
//    os.close();
//}
//
//void testLambda() {
//    function<void(int)> test;
//    test = [&test](int i) {
//        cout << i << endl;
//        if (i < 10)
//            test(i + 1);
//    };
//    test(0);
//}
//
//void testIP() {
//    BoostTCPClientHelper boostTCPClientHelper("219.223.194.92", 8010);
//    boostTCPClientHelper.connect();
//    boostTCPClientHelper.testBlockChainRequest();
//}

int main() {
//    testFile();
//    testLambda();
//    testIP();

}