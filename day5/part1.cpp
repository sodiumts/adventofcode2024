#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>

int middles(const std::vector<std::pair<int, int>> &pairs, const std::vector<std::vector<int>> &updates) {
    
    return 0;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("test.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::vector<std::pair<int, int>> pairs;
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
            pairs.push_back(std::make_pair(firstNum, secondNum));
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
    

        
   std::println("Mas  count: {0}", middles(pairs, updates));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

