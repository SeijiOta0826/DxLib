#pragma once

#include <fstream>
#include <sstream>
#include <string>
# include <vector>
#include <algorithm> // std::remove_if を使うために追加
#include <cctype>    // ::isspace を使うために追加

struct MapData {
    int width;
    int height;
    std::vector<int> tiles;
};

inline bool LoadMapFromCSV(const std::string& filePath, MapData& outData) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    outData.height = 0;
    outData.width = 0;
    outData.tiles.clear();


    // (後略)
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cell;
        int rowWidth = 0;

        while (std::getline(ss, cell, ',')) {
            // 1. 文字列から空白や制御文字（\rや\nなど）を全て削除する
            cell.erase(std::remove_if(cell.begin(), cell.end(), ::isspace), cell.end());

            if (cell.empty()) {
                // 空白マスだった場合は「0（何もない空間）」として格納する
                outData.tiles.push_back(0);
            }
            else {
                // 文字が入っていた場合は数値に変換して格納する
                outData.tiles.push_back(std::stoi(cell));
            }

            rowWidth++;
        }

        if (outData.width == 0) outData.width = rowWidth;
        outData.height++;
    }

    file.close();
    return true;
}