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
}
