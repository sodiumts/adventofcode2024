#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <cctype>
#include <utility>
#include <print>

bool isNum(const std::string &num) {
    for (char c: num) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

int sumResult(const std::string &line) {
    int result = 0;
    std::size_t pos = 0;
    std::size_t doPos = 0;
    std::size_t dontPos = 0;
    bool mulEnabled = true;

    std::vector<int> doPositions;
    while ((doPos = line.find("do()", doPos)) != std::string::npos) {
        doPositions.push_back(doPos);
        doPos += 4;
    }

    std::vector<int> dontPositions;
    while ((dontPos = line.find("don't()", dontPos)) != std::string::npos) {
        dontPositions.push_back(dontPos);
        dontPos += 7;
    }

    std::vector<std::pair<int, int>> dontRanges;
    for (int dont: dontPositions) {
        for (int dos: doPositions) {
            if (dont < dos) {
                dontRanges.push_back(std::make_pair(dont, dos));
                break;
            }
        }
    }
    
    std::print("Don't ranges: ");
    for (auto range: dontRanges) {
        std::print("[{0},{1}], ", range.first, range.second);
    }
    std::println();

    while ((pos = line.find("mul(", pos)) != std::string::npos) {

        std::size_t startP = pos + 4;
        std::size_t endP = line.find(')', startP);

        std::string content = line.substr(startP, endP - startP);
        std::size_t commaP = content.find(',');

        if (commaP == std::string::npos) {
            pos += 4;
            continue;
        }
        std::string firstNum = content.substr(0, commaP);
        std::string secondNum = content.substr(commaP + 1);

        if (isNum(firstNum) && isNum(secondNum)) {
            int first = std::stoi(firstNum);
            int second = std::stoi(secondNum);
            bool isBlocked = false;
            for (auto &range: dontRanges) {
                if(pos > range.first && pos < range.second) {
                    isBlocked = true;
                    break;
                }
            }

            if(!isBlocked){
                result += first * second;
            } 
        }
        pos += 4;
    }
    return result;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::string lines = "";
    std::string line;
    while(std::getline(file, line)) {
        lines += line;
    }
    
    int res = sumResult(lines);

        
    std::println("mul() result: {0}", res);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

