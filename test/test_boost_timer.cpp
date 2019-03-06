//
// Created by mingj on 19-3-6.
//

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;

void show_current_time(const char* what)
{
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout<<what<< std::put_time(std::localtime(&time),"%F %T")<<"\n";
}
//异步定时器
void test_async_deadline_timer()
{
    io_service io;
    deadline_timer timer(io);
    timer.expires_from_now(boost::posix_time::seconds(5));

    show_current_time("before async_wait:");

    timer.async_wait([](const boost::system::error_code& ec){
        if(!ec)
            show_current_time("wait finished:");
    });

    show_current_time("after async_wait:");
//    io.run_one();
}

int main() {
    test_async_deadline_timer();
    cout << "what?" << endl;
}