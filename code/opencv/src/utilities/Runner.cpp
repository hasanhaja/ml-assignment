//
// Created by Hasan on 30/03/20.
//

#include "Runner.h"

namespace util {
    Runner::Runner(const char * train_data, const char * test_data, int no_of_training_samples, int no_of_testing_samples): train_data(train_data), test_data(test_data), no_of_testing_samples(no_of_testing_samples), no_of_training_samples(no_of_training_samples) { }

    auto Runner::run(std::function<int(const char *, const char *, int, int)> method, const char * method_label) -> int {

        std::cout << std::endl;

        std::cout << "[Train data size : Test data size] = [" << no_of_training_samples << " : " << no_of_testing_samples << "]" << std::endl;

        std::cout << "Method [" << method_label << "] starting..." << std::endl;

        auto status = method(train_data, test_data, no_of_training_samples, no_of_testing_samples);

        if (status != 0) {
            std::cerr << "Method [" << method_label << "] failed." << std::endl;
            return -1;
        }

        std::cout << "Method [" << method_label << "] finished." << std::endl;

        std::cout << std::endl;

        return 0;
    }

    Runner::~Runner() = default;
}
