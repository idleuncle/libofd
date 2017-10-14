#include <sys/stat.h> /* mkdir() */
#include <unistd.h> /* write(), close(), access() */
#include <string.h> // strtok
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/time.h>
#include <cstddef>
#include "utils.h"
#include "logger.h"

namespace utils{

    ColorOrder GlobalParameters::colorOrder = ColorOrder::RGB;

    void SetStringStreamPrecision(std::stringstream &ss, int precision){
        ss.setf(std::ios::fixed, std::ios::floatfield); \
            ss.precision(precision);
    }

    std::vector<std::string> SplitString(const std::string& content, const std::string &sep){
        if (sep.empty()){
            std::istringstream iss(content);
            std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, 
                std::istream_iterator<std::string>{}};
            return tokens;
        } else {
            std::string data = content;
            std::replace_if(data.begin(), data.end(), [&](const char &c){if (sep.find(c) != std::string::npos){ return true;} else {return false;}}, ' '); 

            std::istringstream iss(data);
            std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, 
                std::istream_iterator<std::string>{}};
            return tokens;
        }

    }


    std::tuple<char*, size_t, bool> ReadFileData(const std::string &filename){
        bool ok = false;
        char *fontData = nullptr;
        size_t fontDataSize = 0;

        std::ifstream ifile(filename, std::ios::binary);

        ifile.seekg(0, std::ios::end);
        fontDataSize = ifile.tellg();
        LOG_INFO("filename: %s len: %d",  filename.c_str(), fontDataSize);

        fontData = new char[fontDataSize];
        ifile.seekg(0, std::ios::beg);
        ifile.read(fontData, fontDataSize);

        ok = true;

        ifile.close();

        return std::make_tuple(fontData, fontDataSize, ok);
    }


    bool WriteFileData(const std::string &filename, const char *data, size_t dataSize){
        bool ok = true;

        std::ofstream ofile;
        ofile.open(filename, std::ios::binary | std::ios::trunc);
        ofile.write(data, dataSize);
        ofile.close();

        return ok;
    }

    bool FileExist(const std::string &fileName) {
        if ( access(fileName.c_str(), F_OK) == 0 ){
            return true;
        } else {
            return false;
        }
    }

    bool MkdirIfNotExist(const std::string &dirName) {
        if ( !FileExist(dirName) ) {
            return mkdir(dirName.c_str(), 0750);
        } 
        return true;
    }

    unsigned long long GetTimeTick(){
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        return static_cast<unsigned long long>(1000000 * tv.tv_sec + tv.tv_usec);
    }
}


