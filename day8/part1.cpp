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

std::pair<int, int> calculateInterferencePoint(const std::pair<int, int> &pointA, const std::pair<int, int> &pointB) {
    int deltaX = pointA.first - pointB.first;
    int deltaY = pointA.second - pointB.second;

    int pointX = pointA.first  + deltaX;
    int pointY = pointA.second + deltaY;

    return {pointX, pointY};
}

int getUniquePoints(const std::unordered_map<char, std::vector<std::pair<int, int>>> &antennas, int boundX, int boundY) {
    std::unordered_set<std::pair<int, int>, pair_hash> points;
    
    for(auto &entry: antennas) {
        for (int i = 0; i < entry.second.size(); i++) {
            for (int j = i + 1; j < entry.second.size(); j++) {
                std::pair<int, int> pointA = entry.second[i];
                std::pair<int, int> pointB = entry.second[j];
                
                std::pair<int, int> point1 = calculateInterferencePoint(pointA, pointB);
                std::pair<int, int> point2 = calculateInterferencePoint(pointB, pointA);
                
                if (point1.first < boundX && point1.first >= 0 && point1.second < boundY && point1.second >= 0) {
                    points.insert(point1);
                }
                
                if (point2.first < boundX && point2.first >= 0 && point2.second < boundY && point2.second >= 0) {
                    points.insert(point2);
                }
            }
        }
    }
    

    return points.size();
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    
    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    
    std::vector<std::string> lines;
    std::string line;
    while(getline(file, line)) {
        lines.push_back(line);
    }
    
    for(int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if(lines[i][j] != '.') {
                antennas[lines[i][j]].push_back({j, i});            
            }
        }
    }

    std::println("Resonance Point Count: {}", getUniquePoints(antennas, lines[0].size(), lines.size()));
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
