//
// Created by mingj on 19-3-5.
//

#include "UUIDUtils.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

const std::string UUIDUtils::generateUUid() {
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); // 这里是两个() ，因为这里是调用的 () 的运算符重载
    const std::string tmp_uuid = boost::uuids::to_string(a_uuid);
    return tmp_uuid;
}
