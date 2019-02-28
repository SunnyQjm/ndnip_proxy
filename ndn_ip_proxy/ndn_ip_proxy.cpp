//
// Created by mingj on 19-2-27.
//

#include "ndn_ip_proxy.h"
#include "BoostTCPClientHelper.h"
#include "FileUtils.h"
#include <iostream>


int main(int argc, char *argv[]) {
    FileUtils::getResourcePath();
    
    BoostTCPClientHelper boostTCPClientHelper("127.0.0.1", 9748);
    boostTCPClientHelper.connect();
    boostTCPClientHelper.getFileFromServer("ndn.mp4");
//    char *messages[] = {"yangyi", "where", "are", "you", "from"};
//    boost::thread_group threads;
//    for(char **message = messages; *message; ++message) {
//        threads.create_thread([=] { return sync_echo(*message); });
//        boost::this_thread::sleep(boost::posix_time::millisec(1));
//    }
//    threads.join_all();
}
