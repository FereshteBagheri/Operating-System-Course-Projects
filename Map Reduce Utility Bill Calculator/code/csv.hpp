

#ifndef CSV_HPP
#define CSV_HPP
#include "parameters.hpp"




void read_csv(const fs::path& filename, Table& table) {
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::vector<int> row; 
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stoi(cell));
        }
        table.push_back(row);
    }
}

#endif 
