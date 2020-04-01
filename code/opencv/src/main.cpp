/**
 * Created by Hasan on 29/03/20.
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include "utilities/file.h"
#include "utilities/Runner.h"

auto run_tests() -> void {
    // test 1 should be the csv reading test
}

/**
 * Placeholder until runner class is written
 */
auto decision_tree(const char* train_file, const char* test_file, int no_of_training_samples, int no_of_testing_samples) -> int;
auto boost_tree(const char* train_file, const char* test_file, int no_of_training_samples, int no_of_testing_samples) -> int;
auto forest(const char* train_file, const char* test_file, int no_of_training_samples, int no_of_testing_samples) -> int;

auto randomize_and_split(int argc, char** argv) -> int;
auto run_methods(const char* train_file, const char* test_file) -> int;

auto main(int argc, char* argv[]) -> int {

    if (argc < 2) {
        std::cerr << "ERROR: Not enough arguments for the program." << std::endl;
        return -1;
    }

    /**
     * ./progname --dataprep raw.data ad.train ad.test percent
     * ./progname ad.train ad.test
     */
    std::string prep_flag = "--dataprep";

    if (prep_flag.compare(argv[1]) == 0) {
        return randomize_and_split(argc, argv);
    }

    char * train_file = argv[1];
    char * test_file = argv[2];

    //return randomize_and_split(argc, argv);
    return run_methods(train_file, test_file);
}

auto run_methods(const char* train_file, const char* test_file) -> int {

    auto const attributes_per_sample = 1558;

    // ad or nonad
    auto const no_of_classes = 2;
    /**
     * Todo: This is without splitting the data for training and testing
     */
    auto const no_of_nonads = 1978;
    auto const no_of_ads = 381;

    auto training_data_size = util::file::count_lines(train_file);
    auto testing_data_size = util::file::count_lines(test_file);

    util::Runner method(train_file, test_file, training_data_size, testing_data_size);

    /**
     * Method 1 starts and produces results
     */
    method.run(decision_tree, "Binary tree");

    /**
     * Method 2 starts and produces results
     */
    method.run(boost_tree, "Boost tree");

    /**
     * Method 3 starts and produces results
     */
    method.run(forest, "Forest");

    /**
     * ----------- All methods finished -----------
     */

    std::cout << "All the methods have finished. Program exited." << std::endl;
    return 0;
}

/**
 * Run flag and params
 * ./progname --dataprep raw.data ad.train ad.test percent
 * @param argc
 * @param argv
 * @return
 */
auto randomize_and_split(int argc, char* argv[]) -> int {
    using namespace util::file;

    int percentage;

    /**
     * NOTE: argv[1] is the flag --dataprep, so start at argv[2]
     */
    if (argc < 6) {
        std::cerr << "ERROR: Not enough arguments for data preparation." << std::endl;
        return -1;
    }

    char * input_percentage = argv[5];

    try {
        percentage = std::stoi(input_percentage);
        std::cout << "Percentage of data for test = " << percentage << "%" << std::endl;

        if (percentage > 100 || percentage < 0 ) {
            throw std::out_of_range("ERROR: Enter a whole number percentage value between 0 and 100.");
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "ERROR: Enter a whole number percentage value between 0 and 100." << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    std::cout << "Entering data prep mode..." << std::endl;

    if (randomize_data_in_csv(argv[2], argv[3]) != 1) {
        std::cerr << "Randomization failed." << std::endl;
        return -1;
    }

    if (split_csv_data(percentage, argv[3], argv[4]) != 1) {
        std::cerr << "Data splitting failed" << std::endl;
        return -1;
    }

    std::cout << "Data prep complete." << std::endl;
    return 0;
}