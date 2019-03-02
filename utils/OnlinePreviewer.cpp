//
// Created by mingj on 19-3-1.
//

#include "OnlinePreviewer.h"

#include <iostream>

using namespace std;

void OnlinePreviewer::preview(const std::string &filePath) {
    string appname = "smplayer";
    string cmd = appname + " " + filePath;
    system(cmd.data());
}
