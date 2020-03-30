//
// Created by Hasan on 29/03/20.
//

#include "file.h"

namespace util::file {

    namespace {
        auto calculate_max_line(int percentage) -> int {

            int max;

            return max;
        }
    }

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

    /**
     * This function was adapted from the resources provided for the assignment.
     * @param input_file
     * @param output_file
     * @return
     */
    auto randomize_data_in_csv(const char* input_file, const char* output_file) -> int {
        using namespace std;

        vector<char *> inputlines; 				// vector of input lines
        vector<char *>::iterator outline;		// iterator for above

        char * line = nullptr;						// tmp pointer for line memory

        // open input file

        FILE* fi = fopen( input_file, "r" );
        if( !fi ){
            printf("ERROR: cannot read input file %s\n",  input_file);
            return -1; // all not OK
        }

        // open output file

        FILE* fw = fopen( output_file, "w" );
        if( !fw ){
            printf("ERROR: cannot read output file %s\n",  output_file);
            return -1; // all not OK
        }

        // read in all the lines of the file (allocating fresh memory for each)

        while (!feof(fi))
        {
            line = (char *) malloc(LINELENGTHMAX * sizeof(char));
            fscanf(fi, "%[^\n]\n", line);
            inputlines.push_back(line);
        }

        // shuffle input file lines

        random_device rd;
        mt19937 g(rd());

        shuffle(inputlines.begin(), inputlines.end(), g);

        // output all of the lines to output file

        for(outline = inputlines.begin(); outline < inputlines.end(); outline++)
        {
            fprintf(fw, "%s\n", *outline);
            free((void *) *outline); // free memory also
        }

        // close files

        fclose(fi);
        fclose(fw);

        return 1;
    }

    auto split_csv_data(int min_l, int max_l, const char* input_file, const char* output_file) -> int {
        using namespace std;

        vector<char *> inputlines; 				// vector of input lines
        vector<char *>::iterator outline;		// iterator for above

        char * line = nullptr;						// tmp pointer for line memory

        // get min / max line numbers

        int minL = min(min_l, max_l);
        int maxL = max(min_l, max_l);

        int lineN = 0;

        // open input file

        FILE* fi = fopen( input_file, "r" );
        if( !fi ){
            printf("ERROR: cannot read input file %s\n",  input_file);
            return -1; // all not OK
        }

        // open output file

        FILE* fw = fopen( output_file, "w" );
        if( !fw ){
            printf("ERROR: cannot read output file %s\n",  output_file);
            return -1; // all not OK
        }

        // read in all the lines of the file (allocating fresh memory for each)

        while (!feof(fi))
        {
            line = (char *) malloc(LINELENGTHMAX * sizeof(char));
            fscanf(fi, "%[^\n]\n", line);
            inputlines.push_back(line);
        }

        // output seleted lines to output file
        /**
         * Todo, the remainder should go in another file
         */
        for(outline = inputlines.begin(); outline < inputlines.end(); outline++)
        {
            if ((lineN >= minL) && (lineN <= maxL))
            {
                fprintf(fw, "%s\n", *outline);
            }
            lineN++;

            free((void *) *outline); // free memory also
        }

        // close files

        fclose(fi);
        fclose(fw);

        return 1;
    }
}