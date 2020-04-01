% Machine Learning Assignment
% Muhammad Hasan Ali [S324649]
% 30 March 2020

# Abstract

# Contents

# List of figures

# List of tables

# Introduction

## Background

**TODO** The problem at hand has a small number of data points.

### Format of the data

The data was labelled.

**TODO** The number of attributes (3 continuous and the rest binary refers to...) describes...

# Methodology

## Plan

### Tree-based methods

Decision tree, Random Forest or Boost tree. **Decision forest and cascades is close to the state of the art.**
It might be a binary decision tree because of the duality of the classification

- Each node represents an attribute
- Examples to work from: Lab 1, Lab 2 and lab 4
- From Introduction to Machine Learning textbook Nilsson
  > For categorical and numeric attributes, wemust also decide what the tests should be (besides selecting the order). Severaltechniques have been tried; the most popular one is at each stage to select thattest that maximally reduces an entropy-like measure

#### Pros

- Boost introduces weights for each classifier based on performance **citation requires**
- Works best with weak classifiers. **What does this mean?**
- Can learn non-linear relationships [[source](https://elitedatascience.com/machine-learning-algorithms)]

#### Cons

- Unconstrained and prone to overfitting. Can be alleviated using ensembles **What?** [[source](https://elitedatascience.com/machine-learning-algorithms)]
- As tree complexity increases, overfitting starts to occur. Avoid it by post-pruning the tree.
- Boosting increases overfitting

### ANN method

- Examples to work from: Lab 4

#### Pros

- Good performance
- Can be updated with new data using batch propogation [[source](https://elitedatascience.com/machine-learning-algorithms)]

#### Cons

- Requires very large amount of data
- Outperformed by tree ensembles for classical machine learning problems [[source](https://elitedatascience.com/machine-learning-algorithms)]
- Computationally intensive to train

### Bayesian method

If assuming or willing to assume the attributes are independent and normally distributed.

### Method selection

Comparison of the methods:

**TODO** decision matrix

## Data splitting

The data was split into various proportions for training and testing. These proportions were adjusted in 10% intervals to observe the change in accuracy of the system. The function was adapted using the `selectlines.cc` and `randomize.cc` provided with the project to split the provided data. There were a few caveats with this approach:

- The `selectlines.cc` method copies a portion of the source file to a destination file and does not split the file. This had to be refactored so that it splits the data in the destination file in two: the training data and the testing data.
- The `random_shuffle()` function used in `randomize.cc` has been removed since `C++17`, and the alternative `shuffle()` had to be used with a RNG object provided in the `<random>` header file.

There were a total of 2359 data points, of which 1978 were nonads and 381 were ads and they were not shuffled in any fashion. Prior to splitting the data, the data was first shuffled and then split from start to a particular percentage of the file. This ensured that the splitting method used was not only taking one aspect of the data since it was not shuffled when selecting from the top, because the data was shuffled first and it would not make a difference.

# Results and discussion

## Code examples

# Conclusion

# References

# Appendix A: Source code
