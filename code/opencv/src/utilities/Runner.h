//
// Created by Hasan on 30/03/20.
//

#ifndef OPENCV_RUNNER_H
#define OPENCV_RUNNER_H

#include <functional>
#include <iostream>

namespace util {
    class Runner {
    private:
        /**
         * These values should change depending on the dataset
         */
        int no_of_training_samples;
        int no_of_testing_samples;

        const char * train_data;
        const char * test_data;

        /**
         * This should match the function signature of the various methods
         */
        std::function<int(int, char**)> method;

    public:
        Runner(const char* train_data, const char * test_data, int no_of_training_samples = 2359, int no_of_testing_samples = 2359);
        auto run(std::function<int(const char*, const char*)> method, const char* method_label) -> int;

        ~Runner();
    };
}



#endif //OPENCV_RUNNER_H
