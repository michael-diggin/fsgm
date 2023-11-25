#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <Eigen/Dense>
#include "kernelmatrix.h"

typedef std::vector<std::tuple<int,int>> int_tuple_vector;
typedef std::vector<std::tuple<std::string, int>> word_count_vector;

// visited is used as a map/cache
std::unordered_map<std::string, int_tuple_vector> visited;


int count(std::string_view s, std::string_view w)
{
    // TODO: replace with KMP algorithm
    std::size_t count = 0;
    std::size_t pos = s.find(w, 0 );
    while ( pos != std::string_view::npos ) {
        count += 1;
        pos = s.find(w, pos+1 );
    } 
    return count;
}

bool hasSubString(std::string_view s, std::string_view w)
{
    return s.find(w, 0) != std::string::npos;
}



void addToVisited(int index, std::string w, int c)
{
    std::tuple<int, int> indexTupleCount(index, c);
    visited[w].push_back(indexTupleCount);
}

word_count_vector dfsAtWord(std::string s, std::string w, std::vector<std::string> chars, int m, int index)
{
    word_count_vector output;
    if (w.length() == m) {
        int c = count(s, w);
        if (c > 0) {
            addToVisited(index, w, c);
            output.push_back(std::tuple<std::string, int>(w, c));
            return output;
        }
    }

    for (std::string c: chars) {
        if (hasSubString(s, w+c)) {
            word_count_vector temp = dfsAtWord(s, w+c, chars, m, index);
            output.insert(output.end(), temp.begin(), temp.end());
        }
    }

    return output;
}

// ComputeKernelSimilarityMatrix returns the Kernel matrix for the given inputs over a given alphabet
// using the number of overlapping occurences of a substring in the input as the kernel function
Eigen::MatrixXi ComputeKernelSimilarityMatrix(std::vector<std::string> inputs, std::vector<std::string> chars, int m)
{
    visited.clear();
    int input_size = static_cast<int>(inputs.size());
    Eigen::MatrixXi kernel = Eigen::MatrixXi::Zero(input_size, input_size);

    for (int i=0; i<input_size; i++) {
        std::string s = inputs[i];
        for (std::string w : chars) {
            word_count_vector word_counts = dfsAtWord(s, w, chars, m, i);
            for (std::tuple<std::string, int> tup : word_counts) {
                std:: string word = std::get<0>(tup);
                int c1 = std::get<1>(tup);
                for (std::tuple<int, int> index_counts : visited[word]) {
                    int j = std::get<0>(index_counts);
                    int c2 = std::get<1>(index_counts);
                    kernel(i, j) = kernel(i, j) + c1*c2;
                }
            }
        }
    }

    Eigen::MatrixXi diag = kernel.diagonal().asDiagonal();
    return kernel + kernel.transpose() - diag;
}
