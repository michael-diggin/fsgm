#ifndef KERNELMATRIX_H   
#define KERNELMATRIX_H

#include <Eigen/Dense>
#include <vector>
#include <string>
Eigen::MatrixXi ComputeKernelSimilarityMatrix(std::vector<std::string> inputs, std::vector<std::string> chars, int m);

#endif