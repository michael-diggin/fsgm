#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <chrono>
#include <numeric>
#include "kernelmatrix.h"

std::vector<std::string> loadWords()
{
    std::ifstream infile("bench/words.txt");
    std::vector<std::string> words;
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string word;
        if (!(iss >> word)) {
            break;
        }
        words.push_back(word);
    }

    return words;
}

std::vector<std::string> fetchAlphabet(std::vector<std::string> words)
{
    std::set<std::string> alpha;
    for (std::string word : words) {
        for (auto &character : word) {
            std::string s{character};
            alpha.insert(s);
        }
    }
    
    std::vector<std::string> alphabet(alpha.begin(), alpha.end());
    return alphabet;
}

int runBenchmarks(std::vector<std::string> words, std::vector<std::string> alphabet, int m)
{
    std::vector<int> times;
    for (int i = 0; i < 5; i++)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        ComputeKernelSimilarityMatrix(words, alphabet, m);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        times.push_back(ms_int.count());
    }
    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

int main()
{
    std::vector<std::string> words = loadWords();
    std::vector<std::string> alphabet = fetchAlphabet(words);

    std::cout << "Average of running 5 times:" << std::endl;
    for (int i=1;i<11;i++)
    {
        int avgMS = runBenchmarks(words, alphabet, i);
        std::cout << "M=" << i << ": " << avgMS << std::endl;
    }
}