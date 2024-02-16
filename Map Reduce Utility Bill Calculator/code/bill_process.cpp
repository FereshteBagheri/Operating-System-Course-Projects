#include "log.hpp"

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <fcntl.h>
#include "building.hpp"
#include "bill_calculation.hpp"
#include <string>
#include <map>
#include <sstream>
#include <string.h>

namespace fs = std::filesystem;



void get_input(const std::vector<Building>& buildings_with_bill){
    std::cout << "Enter the ID of the building, the resource for which you want to see the parameters, and the month (separated by a space): ";
    int building_id, month;
    std::string resource;
    std::cin >> building_id >> resource >> month;


    if (building_id < 0 || building_id >= buildings_with_bill.size()) {
        LOG(ERROR,"Invalid ID.");
    } else if (resource != "gas" && resource != "water" && resource != "electricity") {
        LOG(ERROR,"Invalid resource.");
    } else if (month < 1 || month > 12) {
        LOG(ERROR,"Invalid month.");
    } else {
        Building wanted_building;
        
        for(const Building& buil : buildings_with_bill){
            if(buil.id == building_id){
               
               wanted_building = buil;
            }
        }
        
        std::map<std::string, Parameters> params = {
            {"gas", wanted_building.gas_params},
            {"water", wanted_building.water_params},
            {"electricity", wanted_building.elec_params}
        };

        std::map<std::string, std::vector<double>> bills = {
            {"gas", wanted_building.gas_bills},
            {"water", wanted_building.water_bills},
            {"electricity", wanted_building.elec_bills}
        };
        
        std::string parameter;
        std::cout << "Enter the parameters you want to see (separated by a space). Available parameters are 'bill', 'total-usage', 'average', 'peak-hour', 'peak-hour-usage'. When finished, press enter: ";
        std::getline(std::cin >> std::ws, parameter);
        std::stringstream ss(parameter);
        std::string item;
        std::map<std::string, std::string> results;
        while (std::getline(ss, item, ' ')) {
            if (item == "bill") {
                results["Bill"] = std::to_string(bills[resource][month - 1]);
            } else if (item == "total-usage") {
                results["Total usage"] = std::to_string(params[resource].total_usage_per_month[month - 1]);
            } else if (item == "average") {
                results["Average usage"] = std::to_string(params[resource].average_usage_per_month);
            } else if (item == "peak-hour") {
                results["Peak hour"] = std::to_string(params[resource].peak_hour_per_month[month - 1]);
            } else if (item == "peak-hour-usage") {
                results["Peak hour usage"] = std::to_string(params[resource].peak_hour_usage_per_month[month - 1]);
            } else {
                std::cout << "Invalid parameter: " << item << ". Please enter a valid parameter.\n";
            }
        }
        std::cerr << "Results:\n";
        for (const auto& result : results) {
            std::cout << result.first << ": " << result.second << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    LOG(INFO,"Bill process.");

    if (argc < 2) {
        LOG(ERROR, "No number of buildings provided");
        return 1;
    }
    std::vector<Building> buildings_with_bill;
    std::vector<std::string> serialized_building_buff; 
    
    int num_buildings = std::stoi(argv[1]);

    std::vector<std::string> named_pipes;
    for (int id = 0; id < num_buildings; id++) {

        std::string pipe_name = "/tmp/bill_pipe_" + std::to_string(id);
        named_pipes.push_back(pipe_name);

    }


    for (const auto& pipe_name : named_pipes) {
        int named_pipe_fd = open(pipe_name.c_str(), O_RDONLY);
        if (named_pipe_fd == -1) {
            LOG(ERROR,"Open error");
            return 1;
        }
        LOG(INFO,"Serialized buidling recieved by bill.");

        char buffer[1024];

       ssize_t numRead;
       std::string serialized_building ;
        while ((numRead = read(named_pipe_fd, buffer, sizeof(buffer) - 1)) > 0)  {
            buffer[numRead] = '\0'; 
            serialized_building += buffer;
            memset(buffer,0,sizeof(buffer));
        }

        Building building_not_calced = deserialize_building(serialized_building);

        calculate_building_bills(building_not_calced);
        buildings_with_bill.push_back(building_not_calced);
        close(named_pipe_fd);
    }

    if(buildings_with_bill.size() == num_buildings){
        get_input(buildings_with_bill);
    }

    return 0;
}

