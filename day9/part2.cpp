#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <print>
#include <utility>
#include <unordered_map>

std::vector<std::vector<int>> getFileIDLength(const std::vector<int> &disk) {
    std::vector<std::vector<int>> idLength;
    int idCounter = 0;
    int idIndex = 0;
    bool alter = false;
    for (int i = 0; i < disk.size(); i++) {
        if(alter){
            idIndex += disk[i];
            alter = false;
        } else {
            idLength.push_back({idCounter, disk[i], idIndex});
            idCounter++;
            idIndex += disk[i];
            alter = true;
        }
    } 

    return idLength;
}

std::vector<std::pair<int,int>> getFreeSpaces(const std::vector<int> &disk) {
    std::vector<std::pair<int,int>> freeSpaces;

    int freeIndex = 0;
    bool alter = false;
    for (int i = 0; i < disk.size(); i++) {
        if (alter) {
            if (disk[i] == 0) {
                alter = false;
                continue;
            }
            freeSpaces.push_back({freeIndex, disk[i]});
            alter = false;
        } else {
            alter = true;
        }
        freeIndex += disk[i];
    }
    return freeSpaces;
}

uint64_t calculateChecksum(const std::vector<int> &diskMap) {
    uint64_t checksum = 0;
    auto idLength = getFileIDLength(diskMap);
    auto freeSpaces = getFreeSpaces(diskMap);
    
    std::reverse(idLength.begin(), idLength.end());
    for(auto &pair: idLength) {
        for (auto &space: freeSpaces) {
            if(pair[1] <= space.second) {
                if(space.first < pair[2]) {

                    pair[2] = space.first;
                    space.second -= pair[1];
                    space.first += pair[1]; 
                    break;
                }
            }
        }
    }

    for (auto &pair: idLength) {
        for (int i = 0; i < pair[1]; i++) {
            checksum += pair[0] * (pair[2] + i);
        }
    }

    return checksum;
}
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("input.txt");
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

    std::println("Checksum: {}", calculateChecksum(diskNum)); 
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::println("Time taken: {0} nanoseconds = {1} milliseconds", duration.count(), duration.count() / 1e6);
    file.close();
    return 0;
}
