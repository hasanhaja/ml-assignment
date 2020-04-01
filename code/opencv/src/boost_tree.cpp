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

#include <string>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace cv::ml;
using namespace util::consts;
using namespace util::file;
/******************************************************************************/

auto boost_tree(const char* train_file, const char* test_file, int no_of_training_samples, int no_of_testing_samples) -> int {

//    int no_of_training_samples = NUMBER_OF_TRAINING_SAMPLES;
//    int no_of_testing_samples = NUMBER_OF_TESTING_SAMPLES;
    
    // define training data storage matrices (one for attribute examples, one
    // for classifications)

    Mat training_data = Mat(no_of_training_samples, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(no_of_training_samples, 1, CV_32FC1);

    //define testing data storage matrices

    Mat testing_data = Mat(no_of_testing_samples, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat testing_classifications = Mat(no_of_testing_samples, 1, CV_32FC1);

    // load training and testing data sets

    if (read_data_from_csv(train_file, training_data, training_classifications, no_of_training_samples) &&
        read_data_from_csv(test_file, testing_data, testing_classifications, no_of_testing_samples)) {

        //*********************************************************************

        // DO YOUR ML HERE

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //
        // As currently boosted tree classifier in OpenCV can only be trained
        // for 2-class problems, we transform the training database by
        // "unrolling" each training sample as many times as the number of
        // classes (10) that we have.
        //
        //  In "unrolling" we add an additional attribute to each training
        //	sample that contains the classification - here 10 new samples
        //  are added for every original sample, one for each possible class
        //	but only one with the correct class as an additional attribute
        //  value has a new binary class of 1, all the rest of the new samples
        //  have a new binary class of 0.
        //
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        Mat new_data = Mat(no_of_training_samples * NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE + 1, CV_32F);
        Mat new_responses = Mat(no_of_training_samples * NUMBER_OF_CLASSES, 1, CV_32S);

        // 1. unroll the training samples

        printf("\nUnrolling the database...");
        fflush(NULL);
        for (int i = 0; i < no_of_training_samples; i++)
        {
            for (int j = 0; j < NUMBER_OF_CLASSES; j++)
            {
                for (int k = 0; k < ATTRIBUTES_PER_SAMPLE; k++)
                {

                    // copy over the attribute data

                    new_data.at<float>((i * NUMBER_OF_CLASSES) + j, k) = training_data.at<float>(i, k);

                }

                // set the new attribute to the original class

                new_data.at<float>((i * NUMBER_OF_CLASSES) + j, ATTRIBUTES_PER_SAMPLE) = (float)j;

                // set the new binary class

                if (((int)training_classifications.at<float>(i, 0)) == j)
                {
                    new_responses.at<int>((i * NUMBER_OF_CLASSES) + j, 0) = 1;
                }
                else
                {
                    new_responses.at<int>((i * NUMBER_OF_CLASSES) + j, 0) = 0;
                }
            }
        }
        printf("Done\n");

        // 2. Unroll the type mask

        // define all the attributes as numerical
        // alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
        // that can be assigned on a per attribute basis

        Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 2, 1, CV_8U);
        var_type.setTo(Scalar(VAR_NUMERICAL)); // all inputs are numerical

        // this is a classification problem (i.e. predict a discrete number of class
        // outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
        // *** the last (new) class indicator attribute, as well
        // *** as the new (binary) response (class) are categorical

        var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = VAR_CATEGORICAL;
        var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE + 1, 0) = VAR_CATEGORICAL;

        // define the parameters for training the boosted trees

        // weights of each classification for classes
        // N.B. in the "unrolled" data we have an imbalance in the training examples
        std::vector<double> priors(2); // weights of each classification for classes
        priors[0] = NUMBER_OF_CLASSES - 1;
        priors[1] = 1;
        //float priors[] = {1,1};

        // set the boost parameters

        Ptr<Boost> boostTree = Boost::create();
        boostTree->setBoostType(Boost::REAL);	// boosting type
        boostTree->setWeakCount(100);					// number of weak classifiers
        boostTree->setWeightTrimRate(0.95);// trim rate
        // trim rate is a threshold (0->1)
        // used to eliminate samples with
        // boosting weight < 1.0 - (trim rate)
        // from the next round of boosting
        // Used for computational saving only.
        boostTree->setMaxDepth(25);						// max depth of trees
        boostTree->setUseSurrogates(false);				// compute surrogate split, no missing data
        boostTree->setPriors(Mat(priors));

        // as CvBoostParams inherits from CvDTreeParams we can also set generic
        // parameters of decision trees too (otherwise they use the defaults)

        boostTree->setMaxCategories(15);// max number of categories (use sub-optimal algorithm for larger numbers)
        boostTree->setMinSampleCount(5);// min sample count
        boostTree->setCVFolds(1);// cross validation folds
        boostTree->setUse1SERule(false);// use 1SE rule => smaller tree
        boostTree->setTruncatePrunedTree(false);// throw away the pruned tree branches
        boostTree->setRegressionAccuracy(0.0);// regression accuracy: N/A here


        // train boosted tree classifier (using training data)

        printf("\nUsing training database: %s\n\n", train_file);
        printf("Training .... (this may take several minutes) .... ");
        fflush(NULL);



        boostTree->train(new_data, ROW_SAMPLE, new_responses);
        printf("Done.");

        // perform classifier testing and report results

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives[NUMBER_OF_CLASSES] = { 0,0 };
        Mat weak_responses = Mat(1, boostTree->getWeakCount(), CV_32F);
        Mat new_sample = Mat(1, ATTRIBUTES_PER_SAMPLE + 1, CV_32F);
        int best_class = 0; // best class returned by weak classifier
        double max_sum;	 // highest score for a given class

        printf("\nUsing testing database: %s\n\n", test_file);

        for (int tsample = 0; tsample < no_of_testing_samples; tsample++)
        {

            // extract a row from the testing matrix

            test_sample = testing_data.row(tsample);

            // convert it to the new "un-rolled" format of input

            for (int k = 0; k < ATTRIBUTES_PER_SAMPLE; k++)
            {
                new_sample.at<float>(0, k) = test_sample.at<float>(0, k);
            }

            // run boosted tree prediction (for N classes and take the
            // maximal response of all the weak classifiers)

            max_sum = INT_MIN; // maximum starts off as Min. Int.

            for (int c = 0; c < NUMBER_OF_CLASSES; c++)
            {
                // set the additional attribute to original class

                new_sample.at<float>(0, ATTRIBUTES_PER_SAMPLE) = (float)c;

                // run prediction (getting also the responses of the weak classifiers)
                // - N.B. here we have to use CvMat() casts and take the address of temporary
                // in order to use the available call that gives us the weak responses
                // For this reason we also have to pass a NULL pointer for the missing data

                boostTree->predict(new_sample, weak_responses, StatModel::Flags::RAW_OUTPUT);

                // obtain the sum of the responses from the weak classifiers

                Scalar responseSum = sum(weak_responses);

                // record the "best class" - i.e. one with maximal response
                // from weak classifiers

                if (responseSum.val[0] > max_sum)
                {
                    max_sum = (double)responseSum.val[0];
                    best_class = c;
                }
            }


            printf("Testing Sample %i -> class result (digit %d)\n", tsample, best_class);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(((float)(best_class)) - testing_classifications.at<float>(tsample, 0))
                >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class

                wrong_class++;

                false_positives[best_class]++;

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
               correct_class, (double)correct_class * 100 / no_of_testing_samples,
               wrong_class, (double)wrong_class * 100 / no_of_testing_samples);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf("\tClass (digit %d) false postives 	%d (%g%%)\n", i,
                   false_positives[i],
                   (double)false_positives[i] * 100 / no_of_testing_samples);
        }

        //*********************************************************************

        // all matrix memory freed by destructors

        // all OK : main returns 0

        return 0;
    }

    // not OK : main returns -1

    return -1;
}