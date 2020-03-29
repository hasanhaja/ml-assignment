//
// Created by Hasan on 29/03/20.
//

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
#include <iostream>

static char CLASSES[2] = { 'N', 'A' };  // class N = 0, class A = 1

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace cv::ml;
using namespace util::consts;
using namespace util::file;
/******************************************************************************/

auto bayes(int argc, char** argv) -> int {

    // define training data storage matrices (one for attribute examples, one
    // for classifications)

    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_64FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_64FC1);

    //define testing data storage matrices

    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_64FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_64FC1);

    // load training and testing data sets

    if (read_data_from_csv(argv[1], training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES) &&
        read_data_from_csv(argv[2], testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES)) {

        //*********************************************************************

        // DO YOUR ML HERE

        // train bayesian classifier (using training data)

        printf("\nUsing training database: %s\n\n", argv[1]);
        Ptr<NormalBayesClassifier> bayes = NormalBayesClassifier::create();

        Ptr<TrainData> trainData = TrainData::create(training_data, SampleTypes::ROW_SAMPLE, training_classifications);

        std::cout << "Here in the code" << std::endl;
        bayes->train(trainData);
        // perform classifier testing and report results

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives[NUMBER_OF_CLASSES];
        char class_labels[NUMBER_OF_CLASSES];
        float result;

        // zero the false positive counters in a simple loop

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            false_positives[i] = 0;
            class_labels[i] = (char)65 + i; // ASCII 65 = A
        }

        printf("\nUsing testing database: %s\n\n", argv[2]);

        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {

            // extract a row from the testing matrix

            test_sample = testing_data.row(tsample);

            // run decision tree prediction

            result = bayes->predict(test_sample);

            printf("Testing Sample %i -> class result (character %c)\n", tsample,
                   CLASSES[((int)result)]);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(result - testing_classifications.at<float>(tsample, 0))
                >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class

                wrong_class++;

                false_positives[((int)result)]++;

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
               argv[2],
               correct_class, (double)correct_class * 100 / NUMBER_OF_TESTING_SAMPLES,
               wrong_class, (double)wrong_class * 100 / NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf("\tClass (character %c) false postives 	%d (%g%%)\n", CLASSES[i],
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

//// Example : normal / naive bayesian learning
//// usage: prog training_data_file testing_data_file
//
//// For use with test / training datasets : other_ex/wdbc.{train|test}
//
//// N.B. *** This bayesian classifier assumes that the attribute (or feature)
//// vectors for each class are normally distributed and independent ***
//// - see OpenCV manual
//
//// "It�s "na�ve" because it assumes that all the features (attributes) are
//// independent from one another even though this is seldom the case
//// (e.g., finding one eye usually implies that another eye is lurking nearby).
//// Zhang discusses possible reasons for the sometimes surprisingly good
//// performance of this classifier [Zhang04]." - Learning OpenCV [Bradski 2009].
//
//// Author : Toby Breckon, toby.breckon@cranfield.ac.uk
//
//// Copyright (c) 2011 School of Engineering, Cranfield University
//// License : LGPL - http://www.gnu.org/licenses/lgpl.html
//
//#include <opencv2/opencv.hpp>       // opencv general include file
//#include <opencv2/ml/ml.hpp>		  // opencv machine learning include file
//
//using namespace cv; // OpenCV API is in the C++ "cv" namespace
//
//#include <stdio.h>
//
///******************************************************************************/
//// global definitions (for speed and ease of use)
//
//#define NUMBER_OF_TRAINING_SAMPLES 449
//#define ATTRIBUTES_PER_SAMPLE 30  // not the first two as patient ID and class
//#define NUMBER_OF_TESTING_SAMPLES 120
//
//#define NUMBER_OF_CLASSES 2
//
//static char CLASSES[2] = { 'B', 'M' };  // class B = 0, class M = 1
//
///******************************************************************************/
//
//// loads the sample database from file (which is a CSV text file)
//
//int read_data_from_csv(const char* filename, Mat data, Mat classes, int n_samples)
//{
//    char tmpc;
//    float tmpf;
//
//    // if we can't read the input file then return 0
//    FILE* f;
//    if (fopen_s(&f, filename, "r"))
//    {
//        printf("ERROR: cannot read file %s\n", filename);
//        return 0; // all not OK
//    }
//
//    // for each sample in the file
//
//    for (int line = 0; line < n_samples; line++)
//    {
//
//        // for each attribute on the line in the file
//
//        for (int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 2); attribute++)
//        {
//            if (attribute == 0)
//            {
//                fscanf_s(f, "%f,", &tmpf);
//
//                // ignore attribute 0 (as it's the patient ID)
//
//                continue;
//            }
//            else if (attribute == 1)
//            {
//
//                // attribute 2 (in the database) is the classification
//                // record 1 = M = malignant
//                // record 0 = B = benign
//
//                fscanf_s(f, "%c,", &tmpc);
//
//                switch (tmpc)
//                {
//                    case 'M':
//                        classes.at<float>(line, 0) = 1.0;
//                        break;
//                    case 'B':
//                        classes.at<float>(line, 0) = 0.0;
//                        break;
//                    default:
//                        printf("ERROR: unexpected class in file %s\n", filename);
//                        return 0; // all not OK
//                }
//
//                // printf("%c,", tmpc);
//            }
//            else
//            {
//                fscanf_s(f, "%f,", &tmpf);
//                data.at<float>(line, (attribute - 2)) = tmpf;
//                //printf("%f,", tmpf);
//            }
//        }
//        fscanf_s(f, "\n");
//        //printf("\n");
//    }
//
//    fclose(f);
//
//    return 1; // all OK
//}
//
///******************************************************************************/
//
//int main(int argc, char** argv)
//{
//    // lets just check the version first
//
//    printf("OpenCV version %s (%d.%d.%d)\n",
//           CV_VERSION,
//           CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
//
//    // define training data storage matrices (one for attribute examples, one
//    // for classifications)
//
//    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
//    Mat training_classifications = Mat::zeros(NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_CLASSES-1, CV_32FC1);
//
//    //define testing data storage matrices
//
//    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
//    Mat testing_classifications = Mat::zeros(NUMBER_OF_TESTING_SAMPLES, NUMBER_OF_CLASSES-1, CV_32FC1);
//
//    // load training and testing data sets
//
//    if (read_data_from_csv(argv[1], training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES) &&
//        read_data_from_csv(argv[2], testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
//    {
//
//        // train bayesian classifier (using training data)
//
//        printf("\nUsing training database: %s\n\n", argv[1]);
//        Ptr<cv::ml::NormalBayesClassifier> bayes = cv::ml::NormalBayesClassifier::create();
//
//        Ptr<cv::ml::TrainData> trainData = ml::TrainData::create(training_data, cv::ml::SampleTypes::ROW_SAMPLE, training_classifications);
//
//        bayes->train(trainData);
//
//        // perform classifier testing and report results
//
//        Mat test_sample;
//        int correct_class = 0;
//        int wrong_class = 0;
//        int false_positives[NUMBER_OF_CLASSES];
//        char class_labels[NUMBER_OF_CLASSES];
//        float result;
//
//        // zero the false positive counters in a simple loop
//
//        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
//        {
//            false_positives[i] = 0;
//            class_labels[i] = (char)65 + i; // ASCII 65 = A
//        }
//
//        printf("\nUsing testing database: %s\n\n", argv[2]);
//
//        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
//        {
//
//            // extract a row from the testing matrix
//
//            test_sample = testing_data.row(tsample);
//
//            // run decision tree prediction
//
//            result = bayes->predict(test_sample);
//
//            printf("Testing Sample %i -> class result (character %c)\n", tsample,
//                   CLASSES[((int)result)]);
//
//            // if the prediction and the (true) testing classification are the same
//            // (N.B. openCV uses a floating point decision tree implementation!)
//
//            if (fabs(result - testing_classifications.at<float>(tsample, 0))
//                >= FLT_EPSILON)
//            {
//                // if they differ more than floating point error => wrong class
//
//                wrong_class++;
//
//                false_positives[((int)result)]++;
//
//            }
//            else
//            {
//
//                // otherwise correct
//
//                correct_class++;
//            }
//        }
//
//        printf("\nResults on the testing database: %s\n"
//               "\tCorrect classification: %d (%g%%)\n"
//               "\tWrong classifications: %d (%g%%)\n",
//               argv[2],
//               correct_class, (double)correct_class * 100 / NUMBER_OF_TESTING_SAMPLES,
//               wrong_class, (double)wrong_class * 100 / NUMBER_OF_TESTING_SAMPLES);
//
//        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
//        {
//            printf("\tClass (character %c) false postives 	%d (%g%%)\n", CLASSES[i],
//                   false_positives[i],
//                   (double)false_positives[i] * 100 / NUMBER_OF_TESTING_SAMPLES);
//        }
//
//        // all matrix memory free by destructors
//
//
//        // all OK : main returns 0
//
//        return 0;
//    }
//
//    // not OK : main returns -1
//
//    return -1;
//}
///******************************************************************************/
