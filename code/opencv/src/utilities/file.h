//
// Created by Hasan on 29/03/20.
//

#ifndef OPENCV_FILE_H
#define OPENCV_FILE_H

#include <cstdio>
#include <random>
#include <opencv2/core/mat.hpp>
#include "constants.h"

namespace util::file {

    // Todo: This could return a Result<Monostate> or a Result<int> and map a std exception
    auto read_data_from_csv(const char* filename, cv::Mat& data, cv::Mat& classes, int n_samples) -> int;
    auto randomize_data_in_csv() -> void;

    // This can be in place
    auto split_csv_data(int min, int max, const char* input_file, const char* output_file) -> void;
    auto write_csv() -> void;
}

#endif //OPENCV_FILE_H
