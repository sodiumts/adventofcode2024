#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>

int getStoneCount(std::vector<uint64_t> &nums, int times) {
    for (int i = 0; i < times; i++) {
        int fixedSize = nums.size();
        for (int j = 0; j < fixedSize; j++) {
            if (nums[j] == 0) {
                nums[j] = 1;
                continue;
            }
            if (nums[j] == 1) {
                nums[j] = 2024;
                continue;
            }

            int numOfDigits = static_cast<int>(std::log10(nums[j]) + 1);
            if (numOfDigits % 2 == 0) {
                int firstHalf = nums[j] / std::pow(10, numOfDigits / 2);
                int secondHalf = nums[j] % static_cast<int>(std::pow(10, numOfDigits / 2));
                nums[j] = firstHalf;
                nums.push_back(secondHalf);
                continue;
            } 
            nums[j] *= 2024;
        }
    } 

    return nums.size();
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    
    std::vector<std::uint64_t> nums;
    std::string line;
    while(getline(file, line)) {
        int idxSpace = line.find(' ');
        nums.push_back(std::stoull(line.substr(0, idxSpace)));
        std::istringstream ss(line.substr(idxSpace + 1));
        uint64_t num;
        while(ss >> num) {
            nums.push_back(num);
        }
    }

    std::println("Number Of Stones: {}", getStoneCount(nums, 25)); 
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
