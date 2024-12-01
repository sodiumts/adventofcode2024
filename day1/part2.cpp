#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <ranges>
#include <cmath>
#include <chrono>
#include <unordered_map>
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }

    std::vector<int> firstCol;

    std::string line;
    std::unordered_map<int, int> occurences;

    while (std::getline(file, line)) {
        bool alt = true; 
        for (auto word : std::views::split(line, std::string_view("   "))) {
            std::string token(&*word.begin(), std::ranges::distance(word));
            int value = 0; 
            try {
                value = std::stoi(token);
            } catch(...) {
                throw std::runtime_error("Failed to parse " + token + " into an int");
            } 
            if (alt) {
                firstCol.push_back(value);
            } else {
                occurences[value]++;
            }
            alt = !alt;
        }
    }
    
    std::sort(firstCol.begin(), firstCol.end());

    int similarityScore = 0;
    for (int i = 0; i < firstCol.size(); i++) {
        similarityScore += firstCol[i] * occurences[firstCol[i]];
    }
    std::cout << "Similarity score: " << similarityScore << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " nanoseconds = " << duration.count() / 1e6 << " milliseconds "<< std::endl;

    return 0;
}

