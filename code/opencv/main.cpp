#include <iostream>
#include <opencv2/opencv.hpp>

int main() {

    cv::Mat img = cv::imread("test.png");

    if (img.empty()) {
        std::cout << "Image cannot be opened." << std::endl;
        return -1;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
