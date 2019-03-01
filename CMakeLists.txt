cmake_minimum_required(VERSION 3.5.1)
project(ndnip_proxy)

set(CMAKE_CXX_STANDARD 14)
include_directories(ip_server)
include_directories(ndn_client)
include_directories(ndn_ip_proxy)
include_directories(utils)
include_directories(protocol)
include_directories(common)

#add_executable(ndnip_proxy main.cpp ip_server/ip_server.cpp ndn_ip_proxy/ndn_ip_proxy.cpp)

add_executable(nip ndn_ip_proxy/ndn_ip_proxy.cpp ndn_ip_proxy/BoostTCPClientHelper.cpp utils/FileUtils.cpp
        utils/JSONCPPHelper.cpp protocol/ProtocolHelper.cpp protocol/BaseProtocolItem.cpp common/common.cpp
        protocol/protocol.cpp utils/NDNHelper.cpp)
target_link_libraries(nip ndn-cxx boost_system boost_thread boost_filesystem jsoncpp pthread)

add_executable(is ip_server/ip_server.cpp ip_server/BoostTCPServerHelper.cpp utils/FileUtils.cpp
        utils/JSONCPPHelper.cpp protocol/ProtocolHelper.cpp protocol/BaseProtocolItem.cpp common/common.cpp
        protocol/protocol.cpp)
target_link_libraries(is boost_system boost_thread boost_filesystem jsoncpp pthread)


add_executable(nc ndn_client/ndn_client.cpp utils/NDNHelper.cpp utils/JSONCPPHelper.cpp protocol/BaseProtocolItem.cpp
        protocol/ProtocolHelper.cpp utils/FileUtils.cpp utils/Previewer.cpp utils/OnlinePreviewer.cpp)
target_link_libraries(nc ndn-cxx boost_system boost_thread boost_filesystem jsoncpp pthread vlc)


###############################################
########## Test
###############################################
add_executable(test_fcw test/test_fstram_cover_write.cpp utils/FileUtils.cpp)
target_link_libraries(test_fcw boost_system boost_filesystem)
#target_link_libraries(ndnip_proxy pcap ndn-cxx boost_system boost_thread jsoncpp pthread)
