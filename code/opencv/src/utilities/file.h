//
// Created by Hasan on 29/03/20.
//

#ifndef OPENCV_FILE_H
#define OPENCV_FILE_H

#include <cstdio>
#include <opencv2/core/mat.hpp>
#include "constants.h"

namespace util::file {

    // Todo: This could return a Result<Monostate> or a Result<int> and map a std exception
    auto read_data_from_csv(const char* filename, cv::Mat& data, cv::Mat& classes, int n_samples) -> int;
}

#endif //OPENCV_FILE_H
