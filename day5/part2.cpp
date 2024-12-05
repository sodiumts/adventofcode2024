#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <unordered_map>
#include <algorithm>

bool isSafe(const std::unordered_map<int, std::vector<int>> &rules, const std::vector<int> &line) {
    for (int i = line.size() - 1; i >= 0; i--) {
        for (int j = i-1; j >= 0; j--) {
            std::vector<int> ref;
            try {
                ref = rules.at(line[j]);
            } catch (const std::out_of_range& e) {
                return false;
            } 
            if(std::find(ref.begin(), ref.end(), line[i]) == ref.end()) {
                return false;
            }
        }
    }
    return true;
}

int fixBadUpdate(const std::unordered_map<int, std::vector<int>> &rules, std::vector<int> &line) {
    bool swapped;

    for (int i = 0; i < line.size() - 1; i++) {
        swapped = false;
        for (int j = 0; j < line.size() - i - 1; j++) {
            std::vector<int> rulesAtNext;
            try {
                rulesAtNext = rules.at(line[j + 1]);
            } catch (const std::out_of_range &e) {
                continue;    
            }
            if (std::find(rulesAtNext.begin(), rulesAtNext.end(), line[j]) != rulesAtNext.end()) {
                int temp = line[j + 1];
                line[j + 1] = line[j];
                line[j] = temp;
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
    //Do a check if line is now valid for debugging
    
    // if(isSafe(rules, line)) {
    //   std::println("This line is safe!");
    // } else {
    //   throw std::runtime_error("Line not safe!");
    // }
    return line[line.size() / 2];
}

int middles(const std::unordered_map<int, std::vector<int>> &rules, std::vector<std::vector<int>> &updates) {
    for(auto [key, value]: rules) {
        std::print("key {0}   :: ", key);
        for (auto val: value) {
            std::print("{0},", val);
        }
        std::println();
    }
    std::println();
    int resultSum = 0;
    for (auto &line: updates) {
        bool is = isSafe(rules, line); 
        if (!is) {
            resultSum += fixBadUpdate(rules, line);
        }
    }
    return resultSum;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::unordered_map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> updates;

    bool firstPart = true;
    std::string line;
    while(std::getline(file, line)) {
        if (line.empty() && std::all_of(line.begin(), line.end(), ::isspace)) {
            firstPart = false;
            continue;
        }
        if (firstPart) {
            int splitter = line.find("|");
            int firstNum = std::stoi(line.substr(0, splitter));
            int secondNum = std::stoi(line.substr(splitter + 1));
            rules[firstNum].push_back(secondNum);
        } else {
            std::size_t pos = 0;
            std::size_t lastPos = 0;
            std::vector<int> nums;
            while((pos = line.find(",", pos)) != std::string::npos) {
                int num = std::stoi(line.substr(lastPos, pos));
                nums.push_back(num);
                int idx = 0;
                lastPos = pos + 1;
                pos++;
            }
            nums.push_back(std::stoi(line.substr(lastPos)));
            updates.push_back(nums);
        }
    }
    
    
        
    std::println("Middle page sum: {0}", middles(rules, updates));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

