#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "log.hpp"

namespace fs = std::filesystem;
using Table = std::vector<std::vector<int>>;

struct Parameters {
    std::vector<int> total_usage_per_month = std::vector<int>(12, 0);
    int average_usage_per_month;
    std::vector<int> peak_hour_per_month = std::vector<int>(12, 0);
    std::vector<int> peak_hour_usage_per_month = std::vector<int>(12, 0);
};

Parameters calculate_parameters(const Table& table);
std::string serialize_param(const Parameters& params);
Parameters deserialize_param(const std::string& s);

Parameters calculate_parameters(const Table& table) {
    Parameters params;

    std::vector<std::vector<int>> usage_by_hour_and_month(12, std::vector<int>(6, 0));

    for (const auto& row : table) {
        int month = row[1] - 1;  // Months are 0-indexed
        for (int hour = 0; hour < 6; hour++) {
            int usage = row[hour + 3];
            params.total_usage_per_month[month] += usage;
            usage_by_hour_and_month[month][hour] += usage;
        }
    }

    params.average_usage_per_month = std::accumulate(params.total_usage_per_month.begin(), params.total_usage_per_month.end(), 0) / 12;

    for (int month = 0; month < 12; month++) {
        auto max_element_iter = std::max_element(usage_by_hour_and_month[month].begin(), usage_by_hour_and_month[month].end());
        params.peak_hour_per_month[month] = std::distance(usage_by_hour_and_month[month].begin(), max_element_iter);
        params.peak_hour_usage_per_month[month] = *max_element_iter;
    }
    return params;
}


    std::string serialize_param(const Parameters& params) {
        std::ostringstream oss;
        for (int i = 0; i < 12; i++) {
            oss << params.total_usage_per_month[i] << (i < 11 ? "," : "/");
        }
        oss << params.average_usage_per_month << "/";
        for (int i = 0; i < 12; i++) {
            oss << params.peak_hour_per_month[i] << (i < 11 ? "," : "/");
        }
        for (int i = 0; i < 12; i++) {
            oss << params.peak_hour_usage_per_month[i] << (i < 11 ? "," : "$");
        }
        return oss.str();
    }


 Parameters deserialize_param(const std::string& s) {
    Parameters params;


    std::istringstream iss(s);
    std::string token;
    for (int i = 0; i < 12; i++) {
        std::getline(iss, token, i < 11 ? ',' : '/');
        params.total_usage_per_month[i] = std::stoi(token);
    }
    std::getline(iss, token, '/');
    params.average_usage_per_month = std::stoi(token);
    for (int i = 0; i < 12; i++) {
        std::getline(iss, token,  i < 11 ? ',' : '/');
        params.peak_hour_per_month[i] = std::stoi(token);
    }
    for (int i = 0; i < 12; i++) {
        std::getline(iss, token, i < 11 ? ',' : '$');
        params.peak_hour_usage_per_month[i] = std::stoi(token);
    }
    return params;
}



#endif 
