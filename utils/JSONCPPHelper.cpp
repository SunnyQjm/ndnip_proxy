//
// Created by mingj on 18-12-22.
//

#include "JSONCPPHelper.h"

JSONCPPHelper::JSONCPPHelper(const string &json) {
    Json::Reader reader;

    //将根节点解析到root当中
    reader.parse(json, this->root);
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
