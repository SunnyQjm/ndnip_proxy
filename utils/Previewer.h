//
// Created by mingj on 19-3-1.
//

#ifndef NDNIP_PROXY_PREVIEWER_H
#define NDNIP_PROXY_PREVIEWER_H


#include <string>

class Previewer {
public:
    virtual void preview(const std::string& filePath) = 0;
};


#endif //NDNIP_PROXY_PREVIEWER_H
