//
// Created by Hasan on 01/04/20.
//

#ifndef OPENCV_LOGGER_H
#define OPENCV_LOGGER_H

#include <string>

class Logger {
private:
    std::string filepath;
    // file object
    auto write_to_file() -> void;
public:
    explicit Logger(std::string filepath);
    auto log() -> void;
    ~Logger();
};


#endif //OPENCV_LOGGER_H
