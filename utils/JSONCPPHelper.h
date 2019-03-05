//
// Created by mingj on 18-12-22.
//
#ifndef IP_NDN_STACK_CPP_JSONCPPHELPER_H
#define IP_NDN_STACK_CPP_JSONCPPHELPER_H

#include <jsoncpp/json/json.h>

using namespace std;

class JSONCPPHelper {
public:
    /**
     * 传入一个JSON格式的字符串，进行解析
     * type = 0  => 解析字符串
     * type = 1  => 解析配置文件
     * @param json
     */
    explicit
    JSONCPPHelper(const string &json, int type = 0);

    string getString(string key);

    int getInt(string key);

    unsigned long long getull(string key);

    bool getBool(string key);

    double getDouble(string key);

    Json::Value get(string key);

private:
    Json::Value root;
};


#endif //IP_NDN_STACK_CPP_JSONCPPHELPER_H
