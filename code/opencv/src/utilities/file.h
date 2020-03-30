//
// Created by Hasan on 29/03/20.
//

#ifndef OPENCV_FILE_H
#define OPENCV_FILE_H

#include <cstdio>
#include <cstdlib>
#include <random>
#include <opencv2/core/mat.hpp>
#include "constants.h"
#include <vector>

namespace util::file {
    const int LINELENGTHMAX = 5000;

    // Todo: This could return a Result<Monostate> or a Result<int> and map a std exception
    auto read_data_from_csv(const char* filename, cv::Mat& data, cv::Mat& classes, int n_samples) -> int;
    auto randomize_data_in_csv(const char* input_file, const char* output_file) -> int;

    // This can be in place
    auto split_csv_data(int max_l, const char* input_file, const char* output_file) -> int;
    //auto write_csv() -> void;
}

#endif //OPENCV_FILE_H
