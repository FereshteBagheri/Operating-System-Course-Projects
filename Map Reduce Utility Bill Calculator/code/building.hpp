#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "parameters.hpp"

struct Building
{
    int id;
    Parameters gas_params;
    Parameters water_params;
    Parameters elec_params;
    std::vector<double> gas_bills = std::vector<double>(12, 0) ;
    std::vector<double> water_bills = std::vector<double>(12, 0);
    std::vector<double> elec_bills = std::vector<double>(12, 0);
};
std::string serialize_building(const Building& building);
Building deserialize_building(const std::string& s);
std::string serialize_building(const Building& building) {
        std::ostringstream oss;
        oss << std::to_string(building.id) <<'@';
        oss << serialize_param(building.gas_params) << '@';
        oss << serialize_param(building.water_params) << '@';
        oss << serialize_param(building.elec_params) << '@';
        for (int i = 0; i < 12; i++) {
            oss << building.gas_bills[i] << (i < 11 ? "," : "@");
        }

        for (int i = 0; i < 12; i++) {
            oss << building.water_bills[i] << (i < 11 ? "," : "@");
        }

        for (int i = 0; i < 12; i++) {
            oss << building.elec_bills[i] << (i < 11 ? "," : "$");
        }
        return oss.str();
    }



    Building deserialize_building(const std::string& s) {
    Building building;
    std::istringstream iss(s);

    std::string token;
    std::getline(iss,token,'@');
    building.id = std::stoi(token);
    std::getline(iss,token,'@');
    building.gas_params = deserialize_param(token);

    std::getline(iss,token,'@');
    building.water_params = deserialize_param(token);

    std::getline(iss,token,'@');
    building.elec_params = deserialize_param(token);

    for (int i = 0; i < 12; i++) {
        std::getline(iss, token, i < 11 ? ',' : '@');
        building.gas_bills[i] = std::stoi(token);
    }

     for (int i = 0; i < 12; i++) {
        std::getline(iss, token, i < 11 ? ',' : '@');
        building.water_bills[i] = std::stoi(token);
    }

     for (int i = 0; i < 12; i++) {
        std::getline(iss, token, i < 11 ? ',' : '$');
        building.elec_bills[i] = std::stoi(token);
    }
    
    return building;
}


#endif 
