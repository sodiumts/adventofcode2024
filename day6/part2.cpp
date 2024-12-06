#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <algorithm>
#include <utility>
#include <unordered_set>
#include <functional>
#include <tuple>
#include <thread>
#include <future>

struct tuple_hash {
    template <class T1, class T2, class T3>
    std::size_t operator()(const std::tuple<T1, T2, T3>& tuple) const {
        auto h1 = std::hash<T1>{}(std::get<0>(tuple));
        auto h2 = std::hash<T2>{}(std::get<1>(tuple));
        auto h3 = std::hash<T3>{}(std::get<2>(tuple));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
struct Guard {
    int x;
    int y;
    int rotation; // 0, 1, 2, 3
};
bool testBlock(std::vector<std::string> &map, struct Guard guard){
    std::unordered_set<std::tuple<int, int, int>, tuple_hash> visitedPositions;
    while(true) {
        std::pair<int, int> nextPosition;
        std::tuple<int, int, int> currentPosition = std::make_tuple(guard.x, guard.y, guard.rotation);
        if(visitedPositions.find(currentPosition) != visitedPositions.end()) {
            return true;
        }
        visitedPositions.insert(currentPosition);
        switch(guard.rotation) {
            case 0:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y - 1;
            break;
            case 1:
                nextPosition.first = guard.x + 1;
                nextPosition.second = guard.y;
            break;
            case 2:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y + 1;
            break;
            case 3:
                nextPosition.first = guard.x - 1;
                nextPosition.second = guard.y;
            break;
        }
        if(nextPosition.first < 0 || nextPosition.first >= map[0].size() || nextPosition.second < 0 || nextPosition.second >= map.size()) {
            break;
        }

        char nextChar = map[nextPosition.second][nextPosition.first];
        if(nextChar == '.') {
            guard.x = nextPosition.first;
            guard.y = nextPosition.second;
        }  else {
            guard.rotation = (guard.rotation + 1) % 4;
        }
    }
    return false;
}

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>()(p.first);
        std::size_t h2 = std::hash<T2>()(p.second);
        return h1 ^ (h2 << 1); // Combine hashes
    }
};

std::unordered_set<std::pair<int, int>, pair_hash> mapOutFirstPath(std::vector<std::string> &map, struct Guard guard) {
    std::unordered_set<std::pair<int, int>, pair_hash> visitedPositions;
    visitedPositions.insert({guard.x, guard.y});
    while(true) {
        std::pair<int, int> nextPosition;
        switch(guard.rotation) {
            case 0:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y - 1;
            break;
            case 1:
                nextPosition.first = guard.x + 1;
                nextPosition.second = guard.y;
            break;
            case 2:
                nextPosition.first = guard.x;
                nextPosition.second = guard.y + 1;
            break;
            case 3:
                nextPosition.first = guard.x - 1;
                nextPosition.second = guard.y;
            break;
        }
        if(nextPosition.first < 0 || nextPosition.first >= map[0].size() || nextPosition.second < 0 || nextPosition.second >= map.size()) {
            break;
        }

        char nextChar = map[nextPosition.second][nextPosition.first];
        if(nextChar == '.') {
            guard.x = nextPosition.first;
            guard.y = nextPosition.second;
            visitedPositions.insert({guard.x, guard.y});
        } else {
           guard.rotation = (guard.rotation + 1) % 4; 
        }
    }
    return visitedPositions;
}

int mapOutPath(std::vector<std::string> &map, struct Guard &guard) {
    auto firstPath = mapOutFirstPath(map, guard);
    
    auto processChunk = [&](auto start, auto end) {
        int lcount = 0;
        for (auto it = start; it != end; it++) {
            std::vector<std::string> copyMap(map);
            copyMap[it->second][it->first] = '#';
            if(testBlock(copyMap, guard)) {
                lcount++;
            }
        }
        return lcount;
    };

    const size_t numThreads = std::thread::hardware_concurrency();
    std::vector<std::future<int>> futures;
    size_t chunkSize = (firstPath.size() + numThreads - 1) / numThreads;

    auto it = firstPath.begin();
    for (size_t i = 0; i < numThreads && it != firstPath.end(); ++i) {
        auto start = it;
        
        size_t distance = static_cast<size_t>(std::distance(it, firstPath.end()));
        
        std::advance(it, std::min(chunkSize, distance));
        auto end = it;

        futures.emplace_back(std::async(std::launch::async, processChunk, start, end));
    }

    int blockCount = 0;
    for (auto &f : futures) {
        blockCount += f.get();
    }

    return blockCount;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }
    
    std::vector<std::string> map;
    std::string line;
    struct Guard guard = { 0, 0, 0 }; 
    int i = 0;
    while(std::getline(file, line)) {
        map.push_back(line);        
        size_t guardIndex = 0;
        if((guardIndex = line.find('^', guardIndex)) != std::string::npos) {
            guard.x = guardIndex;
            guard.y = i;
            map[guard.y][guard.x] = '.';
        }
        i++;
    }
    
        
    std::println("VisitedPositions: {0}", mapOutPath(map, guard));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}

