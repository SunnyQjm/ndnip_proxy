//
// Created by mingj on 18-12-22.
//

#include <iostream>
#include <fstream>
#include "JSONCPPHelper.h"

JSONCPPHelper::JSONCPPHelper(const string &json, int type) {
    Json::Reader reader;
    switch (type) {
        case 0:
            //将根节点解析到root当中
            reader.parse(json, this->root);
            break;
        case 1:
        default:
            //读取文件
            std::ifstream in(json, ios::binary);

            if (!in.is_open()) {
                std::cerr << "config file " << json << " not exist" << std::endl;
                exit(-1);
            }

            //将根节点解析到root当中
            reader.parse(in, this->root);
            in.close();
            break;
    }
}

string JSONCPPHelper::getString(string key) {

    return root[key].asString();
}

int JSONCPPHelper::getInt(string key) {
    return root[key].asInt();
}

unsigned long long JSONCPPHelper::getull(string key) {
    return root[key].asUInt64();
}

bool JSONCPPHelper::getBool(string key) {
    return root[key].asBool();
}

double JSONCPPHelper::getDouble(string key) {
    return root[key].asDouble();
}

Json::Value JSONCPPHelper::get(string key) {
    return root[key];
}
