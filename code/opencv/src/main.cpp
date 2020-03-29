#include <iostream>
#include <opencv2/opencv.hpp>

auto main() -> int {

    /**
     * Todo: What do I have to do with the "3 continous and others binary" information?
     * Todo:
     */
    auto const attributes_per_sample = 1558;

    // ad or nonad
    auto const no_of_classes = 2;
    /**
     * Todo: This is without splitting the data for training and testing
     */
    auto const no_of_nonads = 1978;
    auto const no_of_ads = 381;

    /**
     * Method 1 starts and produces results
     */
    auto method1_name = "Binary tree";
    std::cout << "Method 1 [" << method1_name << "] starting..." << std::endl;

    std::cout << "Method 1 finished." << std::endl;

    /**
     * Method 2 starts and produces results
     */
    auto method2_name = "Boost tree";
    std::cout << "Method 2 [" << method2_name << "] starting..." << std::endl;

    std::cout << "Method 1 finished." << std::endl;

    /**
     * Method 3 starts and produces results
     */
    auto method3_name = "ANN";
    std::cout << "Method 3 [" << method3_name << "] starting..." << std::endl;

    std::cout << "Method 1 finished." << std::endl;

    /**
     * ----------- All methods finished -----------
     */

    std::cout << "All the methods have finished. Program exited." << std::endl;

    return 0;
}
