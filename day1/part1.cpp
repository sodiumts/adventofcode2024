#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ranges>
#include <string_view>

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }

    std::vector<int> firstCol;
    std::vector<int> secondCol;

    std::string line;
    while (std::getline(file, line)) {
        bool alt = true; 
        for (auto word : std::views::split(line, std::string_view("   "))) {
            std::string token(&*word.begin(), std::ranges::distance(word));
            
            try {
                int value = std::stoi(token);
                if (alt) {
                    firstCol.push_back(value);
                } else {
                    secondCol.push_back(value);
                }
                alt = !alt;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid integer '" << token << "' in line: " << line << std::endl;
                return 1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Error: Integer out of range in line: " << line << std::endl;
                return 1;
            }
        }
    }
    
    std::sort(firstCol.begin(), firstCol.end());
    std::sort(secondCol.begin(), secondCol.end());

    int sumOfDifferences = 0;
    for (int i = 0; i < firstCol.size(); i++) {
        sumOfDifferences += std::abs(firstCol[i] - secondCol[i]);
    }
    std::cout << "Sum of differences: " << sumOfDifferences << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " nanoseconds" << std::endl;

    return 0;
}

