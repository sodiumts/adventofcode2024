#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <utility>
#include <unordered_map>
#include <unordered_set>

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>()(p.first);
        std::size_t h2 = std::hash<T2>()(p.second);
        return h1 ^ (h2 << 1);
    }
};



int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("test.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    
    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    
    std::vector<int> diskNum;
    std::string line;
    while(getline(file, line)) {
       for (char c: line) {
            diskNum.push_back(c - '0');
        } 
    }

    for (auto c: diskNum) {
        std::print("{}", c);
    }
    std::println();

    //std::println("Resonance Point Count: {}", );
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
