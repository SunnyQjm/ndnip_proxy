//
// Created by mingj on 19-2-27.
//

#include "ip_server.h"
#include "BoostTCPServerHelper.h"
#include <string>


using namespace std;
int main(int argc, char *argv[]) {
    cout << "main1" << endl;
    BoostTCPServerHelper boostTCPServerHelper(9748);
    boostTCPServerHelper.startListen();
    cout << "main2" << endl;
}