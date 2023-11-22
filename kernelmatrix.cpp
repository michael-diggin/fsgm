#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>
#include <Eigen/Dense>

typedef std::vector<std::tuple<int,int>> int_tuple_vector;
typedef std::vector<std::tuple<std::string, int>> word_count_vector;

// visited is used a map/cache
std::unordered_map<std::string, int_tuple_vector> visited;


int count(std::string s, std::string w)
{
   int occurrences = 0;
   std::string::size_type pos = 0;
   while ((pos = s.find(w, pos)) != std::string::npos) {
          ++ occurrences;
          pos += w.length();
   }
   return occurrences;
}

bool hasSubString(std::string s, std::string w)
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
    Eigen::MatrixXi kernel(inputs.size(), inputs.size());

    for (int i=0; i<inputs.size(); i++) {
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
    visited.clear();

    Eigen::MatrixXi diag = kernel.diagonal().asDiagonal();
    return kernel + kernel.transpose() - diag;
}