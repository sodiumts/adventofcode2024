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

std::unordered_set<std::pair<int, int>, pair_hash> calculateInterferencePoint(const std::pair<int, int> &pointA, const std::pair<int, int> &pointB, int boundX, int boundY) {
    std::unordered_set<std::pair<int, int>, pair_hash> intPoints;
    int deltaX = pointA.first - pointB.first;
    int deltaY = pointA.second - pointB.second;
    
    int pointX = pointA.first;
    int pointY = pointA.second;
    while(true) {
        pointX = pointX  + deltaX;
        pointY = pointY + deltaY;
        if (pointX >= 0 && pointX < boundX && pointY >= 0 && pointY < boundY) {
            intPoints.insert({pointX, pointY});
        } else {
            break;
        }
    }
    
    pointX = pointA.first;
    pointY = pointA.second;
    while(true) {
        pointX = pointX  - deltaX;
        pointY = pointY - deltaY;
        if (pointX >= 0 && pointX < boundX && pointY >= 0 && pointY < boundY) {
            intPoints.insert({pointX, pointY});
        } else {
            break;
        }
    }


    return intPoints;
}

int getUniquePoints(const std::unordered_map<char, std::vector<std::pair<int, int>>> &antennas, int boundX, int boundY) {
    std::unordered_set<std::pair<int, int>, pair_hash> points;
    
    for(auto &entry: antennas) {
        for (int i = 0; i < entry.second.size(); i++) {
            for (int j = i + 1; j < entry.second.size(); j++) {
                std::pair<int, int> pointA = entry.second[i];
                std::pair<int, int> pointB = entry.second[j];
            
                auto p1 = calculateInterferencePoint(pointA, pointB, boundX, boundY);
                auto p2 = calculateInterferencePoint(pointB, pointA, boundX, boundY);
                for (auto &p: p1) {
                    points.insert(p);
                }
                for (auto &p: p2) {
                    points.insert(p);
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
