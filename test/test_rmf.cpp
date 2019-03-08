//
// Created by mingj on 19-3-8.
//

#include "FileUtils.h"
#include "ResourceManagerFactory.h"
#include <iostream>
using namespace std;
int main() {
    auto path = FileUtils::getCurrentPath()
            .append("config")
            .append("ndn_client_config.json");
    boost::filesystem::fstream fs(path, std::ios_base::binary | std::ios_base::in);
    cout << fs.is_open() << endl;
    {
        mingj::factory::ResourceManagerFactory<boost::filesystem::fstream> rmf(fs, [](boost::filesystem::fstream& fstream) {
            cout << "callback: " << fstream.is_open() << endl;
            fstream.close();
        });
    }
    cout << "程序结束: " << fs.is_open() << endl;

//    mingj::manager::FStreamManager fsm(filePath, std::ios_base::binary | std::ios_base::in);

}