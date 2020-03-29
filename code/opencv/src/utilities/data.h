//
// Created by Hasan on 29/03/20.
//

#ifndef OPENCV_DATA_H
#define OPENCV_DATA_H

#include <opencv2/core/mat.hpp>

namespace util::data {
    struct Input {
        cv::Mat data;
        cv::Mat classes;
    };
}

#endif //OPENCV_DATA_H
