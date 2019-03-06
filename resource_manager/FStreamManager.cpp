//
// Created by mingj on 19-3-6.
//

#include "FStreamManager.h"
#include <iostream>

namespace mingj {
    namespace manager {
        using namespace std;

        FStreamManager::FStreamManager(const boost::filesystem::path &p, std::ios_base::openmode mode) :
                fs(p, mode) {
//            cout << "is_open: " << fs.is_open() << endl;
        }


        FStreamManager::~FStreamManager() {
            try {
//                cout << "析构->is_open: " << fs.is_open() << endl;
                if (fs.is_open()) {
//                    std::cout << "析构关闭" << std::endl;
                    fs.close();
                }
            } catch (std::exception &e) {
                std::cerr << "FStreamManager: " << e.what() << std::endl;
            }
        }

        bool FStreamManager::is_open() {
            return fs.is_open();
        }

        bool FStreamManager::eof() const {
            return fs.eof();
        }

        basic_istream<char, std::char_traits<char>>::__istream_type &FStreamManager::read(char *__s, size_t __n) {
            return fs.read(__s, __n);
        }

        streamsize FStreamManager::gcount() const {
            return fs.gcount();
        }

        basic_istream<char, std::char_traits<char>>::__istream_type &
        FStreamManager::seekg(size_t offset, std::ios_base::seekdir dir) {
            return fs.seekg(offset, dir);
        }
    }
}