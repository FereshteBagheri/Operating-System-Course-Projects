

#include <iostream>
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <fcntl.h>
#include "parameters.hpp"
#include "log.hpp"

namespace fs = std::filesystem;

int main() {
    LOG(INFO,"Initial process.");
    int num_buildings = 0;
    fs::path buildings_dir = "buildings";

    for (const auto& entry : fs::directory_iterator(buildings_dir)) {
        if (entry.is_directory()){
            num_buildings++;
        }
    }
    LOG(INFO, "Found "+std::to_string(num_buildings) + " buildings.");

    pid_t bill_pid = fork();
    if (bill_pid < 0){
        LOG(ERROR,"Fork of bill process faild.");
    }
    else if (bill_pid == 0) { 
        LOG(INFO,"Bill forked.");
        execl("./bill_process", "./bill_process", std::to_string(num_buildings).c_str(), NULL);
        LOG(ERROR,"execl of bill process faild.");
        return 1;
    }

    int id = 0;
    for (const auto& entry : fs::directory_iterator(buildings_dir)) {
        if (entry.is_directory()){

            fs::path building_dir = entry.path();
            pid_t pid = fork();

            if (pid == -1) {
                LOG(ERROR,"Fork of building process failed");
                return 1;
            }

        if (pid == 0) { 
            LOG(INFO,"Building forked.");
            execl("./building_process", "./building_process", building_dir.c_str(), std::to_string(id).c_str(), NULL);
            LOG(ERROR,"execl of building process faild.");
            return 1;
        }
        id++;
        }
    }

    int status;
    while (wait(&status) != -1);

    return 0;
}