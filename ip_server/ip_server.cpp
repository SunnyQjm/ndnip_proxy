//
// Created by mingj on 19-2-27.
//

#include "ip_server.h"
#include "BoostTCPServerHelper.h"


using namespace std;
int main(int argc, char *argv[]) {
    BoostTCPServerHelper boostTCPServerHelper(9758);
    boostTCPServerHelper.startListen();
}