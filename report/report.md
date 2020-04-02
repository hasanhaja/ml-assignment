% Machine Learning Assignment
% Muhammad Hasan Ali [S324649]
% 30 March 2020

# Abstract

The random forest method did not yield any useful results and the classifier failed even after tweaking all parameters.

# Contents

# List of figures

# List of tables

# Introduction

The problem solved using this assignment is to classify where images extracted from websites are advertisement or not. The dataset provided was labelled and unshuffled and it contains a total of 2359 data points. The presence of labelled data gives way for the use of supervised learning methods [[source](https://towardsdatascience.com/supervised-vs-unsupervised-learning-14f68e32ea8d)]. There are a number of supervised learning methods and they include decision tree based methods, support vector machines and linear regression methods [[source](https://machinelearningmastery.com/supervised-and-unsupervised-machine-learning-algorithms/)].

## Tree-based methods

There are numerous decision tree based methods. The simple one with a dual classification would be a Binary Decision tree, and a generalized version of that would be a Decision tree. Each node, or leaf, in the tree represents an attribute. Derivatives of a Decision tree include Random Forest and Boosting Decision tree. Of this class of methods, Random forests is among the state of the art.

### Pros

- Boosting decision trees introduce weights for each classifier based on performance **citation requires**
- These work best with weak classifiers. **What does this mean?**
- They are also capable of learning non-linear relationships [[source](https://elitedatascience.com/machine-learning-algorithms)]

### Cons

- Decision trees are unconstrained and are prone to overfitting. This can be alleviated using ensemble methods like Random Forests or Boosting Decision Trees [[source](https://elitedatascience.com/machine-learning-algorithms)]
- As tree complexity increases, overfitting starts to occur. It can be avoided by post-pruning the tree.

## Artificial Neural Networks

### Pros

- It can have good performance depending on the implementation.
- It can be updated with new data using batch propogation [[source](https://elitedatascience.com/machine-learning-algorithms)]

### Cons

- It is outperformed by tree ensembles for classical machine learning problems [[source](https://elitedatascience.com/machine-learning-algorithms)]
- It is computationally intensive to train.

# Methodology

The methods explored in this project are all supervised learning methods.

## Developmental setup and execution

The evironment used clion, cmake and complied on windows using the CMake GUI tool.
opencv 4.20
C++17 features

**How to run the program?**
The easiest way to compile the source code is to use CMake with OpenCV 4.2.0 already setup on the system. To build via the terminal, navigate to the directory `ml-assignment/code/opencv` which contains the `CMakeLists.txt` file. Then run the tool using `ccmake .` in the current directory, and then run `make` [[source](https://cmake.org/runningcmake/)]. This will produce an executable file that can be run as per the assignment requirement: `./progname ad.train ad.test`.

An additional feature is that the program can also be used to split the data into the required training and test datasets like this: `./progname --dataprep raw.data <dest_training_file> <dest_testing_file> <percentage_split>`. For example: `./progname --dataprep raw.data ad3070.train ad3070.test 30`.

> It will overwrite files of the same name and create files if it does not exist.

## Plan

> Noise was not explicitly considered. I am uncertain if the default functions and parameters account for noise in some fashion.

The methods selected were Binary Decision Tree, Boosting Decision Tree and Random Forests. This would serve as an exercise in comparing the differences between the methods and the differences in performance.

## Data splitting

The data was split into various proportions for training and testing. These proportions were adjusted in 10% intervals to observe the change in accuracy of the system. The function was adapted using the `selectlines.cc` and `randomize.cc` provided with the project to split the provided data. There were a few caveats with this approach:

- The `selectlines.cc` method copies a portion of the source file to a destination file and does not split the file. This had to be refactored so that it splits the data in the destination file in two: the training data and the testing data.
- The `random_shuffle()` function used in `randomize.cc` has been removed since `C++17`, and the alternative `shuffle()` had to be used with a RNG object provided in the `<random>` header file. The RNG object used was the Mersenne Twister generator, and it is a strong and widely used pseudorandom number generator [[source](https://www.sciencedirect.com/topics/computer-science/mersenne-twister)].

There were a total of 2359 data points, of which 1978 were nonads and 381 were ads and they were not shuffled in any fashion. Prior to splitting the data, the data was first shuffled and then split from start to a particular percentage of the file. This ensured that the splitting method used was not only taking one aspect of the data since it was not shuffled when selecting from the top, because the data was shuffled first and it would not make a difference.

Since the randomization process occurs everytime the data is split, the splitting into various ratios occurred first and saved. This enables consistency between my testing environment and the examiners. The approach used to generate various datasets was by generating the following splits: 10-90, 20-80, 30-70, 40-60 and 50-50. For the following datasets, the order in which the files were supplied to the program was swapped (i.e. `./progname ad.train ad.test` to `./progname ad.test ad.train`), and for the 50-50 dataset, the results were averaged.

## Parameter tweaking

All of the parameters were tweaked using the 30-70 data split.

The weights selected for this approach were...

# Results

## Qualitative observations

The primary development environment used was a Macbook Pro (late 2013 with 8GB of RAM) with the CLion IDE that uses CMake to build the project. The compilation times was not comparable but the runtime performance was noticeable. Even though I was not able to measure the runtime quantitatively, the difference in runtime between the three methods was very noticeable. The Binary Decision tree trained and produced the test results the quickest, whereas the Boosting Decision tree method was the slowest; the margin between the two was vast.

## Quantitative observations

- comparison of data splits
- comparison between methods

# Discussion

**Why wasn't the Random Forest method good?**

## Code examples

# Conclusion

# References

# Appendix A: Source code
