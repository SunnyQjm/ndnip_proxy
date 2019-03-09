//
// Created by mingj on 19-3-9.
//

#include "TimeUtils.h"
#include <chrono>

long TimeUtils::getCurTime() {
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
                    .time_since_epoch());
    return duration_in_ms.count();
}
