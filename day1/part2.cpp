#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <ranges>
#include <cmath>
#include <chrono>
#include <numeric>
#include <unordered_map>

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line)) {
        lines.push_back(line);
    }

    std::vector<int> firstCol;
    firstCol.reserve(lines.size());
    std::unordered_map<int, int> occurences;
    
    for (auto l : lines) {
        auto position = l.find_first_of(" \t");
        auto leftVal = std::stoi(l.substr(0, position));
        auto rightVal = std::stoi(l.substr(position + 1));
        firstCol.push_back(leftVal);
        occurences[rightVal]++;
    }

    int similarityScore = std::accumulate(firstCol.begin(), firstCol.end(), 0, 
                             [&occurences](int accumulator, int value) {
                                            return accumulator + value * occurences[value];
                                          });
    
    std::cout << "Similarity score: " << similarityScore << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " nanoseconds = " << duration.count() / 1e6 << " milliseconds "<< std::endl;
    file.close();
    return 0;
}

