//
// Created by mingj on 19-3-1.
//

#ifndef NDNIP_PROXY_ONLINEPREVIEWER_H
#define NDNIP_PROXY_ONLINEPREVIEWER_H

#include "Previewer.h"

class OnlinePreviewer: public Previewer{
public:
    void preview(const std::string &filePath) override;
};


#endif //NDNIP_PROXY_ONLINEPREVIEWER_H
