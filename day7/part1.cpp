#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>


bool testIfPossible(const std::vector<uint32_t> &numbers, uint32_t result, int index, uint32_t current) {
    if (index == numbers.size())
        return current == result;

    if (testIfPossible(numbers, result, index + 1, current + numbers[index]))
        return true;

    if (testIfPossible(numbers, result, index + 1, current * numbers[index]))
        return true;

    return false;
}

uint32_t getCalibrationResult(const std::vector<std::vector<uint32_t>> &numbers) {
    uint32_t result = 0;

    for (const auto &numLine : numbers) {
        uint32_t res = numLine[0];
        std::vector<uint32_t> list(numLine.begin() + 1, numLine.end());
        if (testIfPossible(list, res, 0, 0))
            result += res;
    }

    return result;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("test.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::vector<std::vector<uint32_t>> numbers;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        std::vector<uint32_t> numLine;

        int indexFirst = line.find(":");
        if (indexFirst == std::string::npos) {
            continue;
        }
        uint32_t number = std::stoul(line.substr(0, indexFirst));
        numLine.push_back(number);

        std::istringstream ss(line.substr(indexFirst + 1));
        uint32_t num;
        while (ss >> num) {
            numLine.push_back(num);
        }

        for (uint32_t num : numLine) {
            std::print("{}, ", num);
        }
        std::println();

        numbers.push_back(numLine);
    }

    uint32_t totalResult = getCalibrationResult(numbers);
    std::println("Total Calibration Result: {}", totalResult);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;

    return 0;
}
