#ifndef BILL_CALCULATION_HPP
#define BILL_CALCULATION_HPP

const double CO = 1.0;

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <fcntl.h>
#include <sstream>
#include <numeric>
#include "parameters.hpp"
#include "log.hpp"
#include "building.hpp"





std::vector<double> calculate_water_bill(const Parameters& water_params, double coefficient);
std::vector<double> calculate_gas_bill(const Parameters& gas_params, double coefficient);
std::vector<double> calculate_elec_bill(const Parameters& power_params, double coefficient);
void calculate_building_bills( Building& building);


std::vector<double> calculate_water_bill(const Parameters& water_params, double coefficient) {
    std::vector<double> bill_per_month;
    double total_water_bill = 0.0;
    for (double i = 0; i < 12; i++) {
        total_water_bill = water_params.total_usage_per_month[i]*coefficient + water_params.peak_hour_usage_per_month[i]*0.25;
        bill_per_month.push_back(total_water_bill);
    }
    return bill_per_month;
}

std::vector<double> calculate_gas_bill(const Parameters& gas_params, double coefficient) {
    std::vector<double> bill_per_month;
    double total_gas_bill = 0.0;
    for (double i = 0; i < 12; i++) {
        total_gas_bill = gas_params.total_usage_per_month[i]*coefficient;
        bill_per_month.push_back(total_gas_bill);
    }
    return bill_per_month;
}

std::vector<double> calculate_elec_bill(const Parameters& power_params, double coefficient) {
    double avrage = power_params.average_usage_per_month;
    std::vector<double> bill_per_month;
    double total_power_bill = 0.0;
    for (double i = 0; i < 12; i++) {
        if(power_params.total_usage_per_month[i] < avrage){
            total_power_bill = ((power_params.total_usage_per_month[i]-power_params.peak_hour_usage_per_month[i])*0.75 + power_params.peak_hour_usage_per_month[i]* 1.25);
        }
        else {
            total_power_bill = power_params.total_usage_per_month[i]*coefficient + power_params.peak_hour_usage_per_month[i]*0.25;
        }

        bill_per_month.push_back(total_power_bill);
    }
    return bill_per_month;
}

void calculate_building_bills( Building& building){
    building.gas_bills = calculate_gas_bill(building.gas_params,CO);
    building.elec_bills = calculate_elec_bill(building.elec_params,CO);
    building.water_bills = calculate_water_bill(building.water_params,CO);

}





#endif