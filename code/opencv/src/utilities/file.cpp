//
// Created by Hasan on 29/03/20.
//

#include "file.h"

namespace util::file {
    auto read_data_from_csv(const char* filename, cv::Mat& data, cv::Mat& classes, int n_samples) -> int {

        using namespace util::consts;
        using namespace std;

        char tmps[10]; // tmp string for reading the "ad." and "nonad." class labels

        // if we can't read the input file then return 0
        FILE* f = fopen( filename, "r" );
        if( !f )
        {
            printf("ERROR: cannot read file %s\n",  filename);
            return 0; // all not OK
        }

        // for each sample in the file

        for(int line = 0; line < n_samples; line++)
        {

            // for each attribute on the line in the file

            for(int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
            {
                if (attribute == ATTRIBUTES_PER_SAMPLE)
                {

                    // last attribute is the class

                    fscanf(f, "%s.\n", tmps);

                    if (strcmp(tmps, "ad.") == 0)
                    {
                        // adverts are class 1

                        classes.at<float>(line, 0) = 1.0;
//                        classes.at<double>(line, 0) = 1.0;

                    }
                    else if (strcmp(tmps, "nonad.") == 0)
                    {
                        // non adverts are class 2

                        classes.at<float>(line, 0) = 0.0;
//                        classes.at<double>(line, 0) = 0.0;
                    }
#if PRINT_CSV_FILE_INPUTS
                    printf("%s\n", tmps);
#endif
                }
                else
                {

                    // store all other data as floating point

                    fscanf(f, "%f,", &(data.at<float>(line, attribute)));
//                    fscanf(f, "%lf,", &(data.at<double>(line, attribute)));
#if PRINT_CSV_FILE_INPUTS
                    printf("%f,", data.at<float>(line, attribute));
//                    printf("%lf,", data.at<double>(line, attribute));
#endif
                }
            }
        }

        fclose(f);

        return 1; // all okay
    }

}