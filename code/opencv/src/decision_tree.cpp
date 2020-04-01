//
// Created by Hasan on 29/03/20.
//


// Example : ML assignment data reader 2011
// usage: prog training_data_file testing_data_file

// For use with testing/training datasets of the same format as: ad_cranfield.data

// for simple test of data reading run as "reader ad_cranfield.data ad_cranfield.data"
// and set defintion below to 1 to print out input data

#define PRINT_CSV_FILE_INPUTS 0

/******************************************************************************/

#include <opencv2/opencv.hpp>       // opencv general include file
#include "utilities/constants.h"
#include "utilities/file.h"

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace cv::ml;
using namespace util::consts;
using namespace util::file;
/******************************************************************************/

auto decision_tree(const char* train_file, const char* test_file) -> int {

    // define training data storage matrices (one for attribute examples, one
    // for classifications)

    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_32FC1);

    //define testing data storage matrices

    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

    // define all the attributes as numerical (** not needed for all ML techniques **)

    Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
    var_type.setTo(Scalar(VAR_NUMERICAL) ); // all inputs are numerical

    // this is a classification problem so reset the last (+1) output
    // var_type element to CV_VAR_CATEGORICAL (** not needed for all ML techniques **)

    var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = VAR_CATEGORICAL;

    // load training and testing data sets

    if (read_data_from_csv(train_file, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES) &&
        read_data_from_csv(test_file, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES)) {

        //*********************************************************************

        // DO YOUR ML HERE

        // define the parameters for training the decision tree
        std::vector<double> priors(2); // weights of each classification for classes
        priors[0] = 1;
        priors[1] = 1;
        // weights of each classification for classes
        // (all equal as equal samples of each digit)

        Ptr<DTrees> dtree = DTrees::create();
        dtree->setMaxDepth(25);// max depth
        dtree->setMinSampleCount(5);// min sample count
        dtree->setRegressionAccuracy(0);// regression accuracy: N/A here
        dtree->setUseSurrogates(false);// compute surrogate split, no missing data
        dtree->setMaxCategories(15);// max number of categories (use sub-optimal algorithm for larger numbers)
        dtree->setCVFolds(1);// the number of cross-validation folds
        dtree->setUse1SERule(false);// use 1SE rule => smaller tree
        dtree->setTruncatePrunedTree(false);// throw away the pruned tree branches
        dtree->setPriors(Mat(priors));// the array of priors

        // train decision tree classifier (using training data)

        printf("\nUsing training database: %s\n\n", train_file);

        dtree->train(training_data, ROW_SAMPLE, training_classifications);

        // perform classifier testing and report results

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives[NUMBER_OF_CLASSES] = { 0,0 };

        printf("\nUsing testing database: %s\n\n", test_file);

        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {

            // extract a row from the testing matrix

            test_sample = testing_data.row(tsample);

            // run decision tree prediction

            float result = dtree->predict(test_sample, noArray(), StatModel::Flags::RAW_OUTPUT);

            printf("Testing Sample %i -> class result (digit %d)\n", tsample, (int)result);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(result - testing_classifications.at<float>(tsample, 0))
                >= FLT_EPSILON)

            {
                // if they differ more than floating point error => wrong class

                wrong_class++;

                false_positives[(int)result]++;

            }
            else
            {

                // otherwise correct

                correct_class++;
            }
        }

        printf("\nResults on the testing database: %s\n"
               "\tCorrect classification: %d (%g%%)\n"
               "\tWrong classifications: %d (%g%%)\n",
               test_file,
               correct_class, (double)correct_class * 100 / NUMBER_OF_TESTING_SAMPLES,
               wrong_class, (double)wrong_class * 100 / NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf("\tClass (digit %d) false postives 	%d (%g%%)\n", i,
                   false_positives[i],
                   (double)false_positives[i] * 100 / NUMBER_OF_TESTING_SAMPLES);
        }



        //*********************************************************************

        // all matrix memory freed by destructors

        // all OK : main returns 0

        return 0;
    }

    // not OK : main returns -1

    return -1;
}