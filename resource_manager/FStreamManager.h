//
// Created by mingj on 19-3-6.
//

#ifndef NDNIP_PROXY_FSTREAMMANAGER_H
#define NDNIP_PROXY_FSTREAMMANAGER_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace mingj {
    namespace manager {
        class Uncopyable {
        protected:// allow construction
            Uncopyable() {}// and destruction of
            ~Uncopyable() {}// derived objects...

        private:
            Uncopyable(const Uncopyable &);// ...but prevent copying
            Uncopyable &operator=(const Uncopyable &);
        };


        class FStreamManager : private Uncopyable {
        public:
            explicit
            FStreamManager(const boost::filesystem::path &p, std::ios_base::openmode mode);


            // 包裹函数
            bool is_open();

            bool eof() const;

            std::basic_istream<char, std::char_traits<char>>::__istream_type &read(char *__s, size_t __n);

            std::streamsize gcount() const;

            std::basic_istream<char, std::char_traits<char>>::__istream_type &
            seekg(size_t offset, std::ios_base::seekdir dir);


            boost::filesystem::fstream &get() {

                return fs;
            }

            ~FStreamManager();

        private:
            boost::filesystem::fstream fs;
        };
    }
}


#endif //NDNIP_PROXY_FSTREAMMANAGER_H
