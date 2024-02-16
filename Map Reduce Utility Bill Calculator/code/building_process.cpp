#include "log.hpp"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <fcntl.h>
#include <sys/stat.h> 
#include <numeric>
#include "building.hpp"
#include "parameters.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    LOG(INFO,"Building process.");
    
    if (argc < 3) {
        LOG(ERROR, "No building directory or Id provided");
        return 1;
    }
    

// Create a named pipe

    fs::path building_dir = argv[1];
    int id = std::stoi(argv[2]);
    
    std::vector<std::string> csv_files = {"Gas.csv", "Water.csv", "Electricity.csv"};
    std::vector<std::string> serialized_params_buff;
    std :: vector<std::string> params_to_bill;
    Building curr_building;
    curr_building.id = id;


    for (const auto& csv_file : csv_files) {
        int fd[2];

        if (pipe(fd) == -1) {
            LOG(ERROR," Unnamed pipe creation failed.");
            return 1;
        }

        pid_t pid = fork();

        if (pid == -1) {
            LOG(ERROR,"Fork of Resource process failed.");
            return 1;
        }

        if (pid == 0) {
            LOG(INFO,"Resource process forked.");
            close(fd[0]); 

    
            dup2(fd[1], STDOUT_FILENO);  
            execl("./resource_process", "./resource_process", (building_dir / csv_file).c_str(), NULL);

            LOG(ERROR, "Resource process exec failed.");
            return 1;
        }

        close(fd[1]);
        LOG(INFO,"Building closed write end.");

        char buffer[1024];

        ssize_t numRead;
        while ((numRead = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[numRead] = '\0';
            serialized_params_buff.push_back(buffer);
        }
        std::string serialized_param= std::accumulate(serialized_params_buff.begin(), serialized_params_buff.end(), std::string());
        if(csv_file=="Gas.csv"){
            curr_building.gas_params = deserialize_param(serialized_param);
        }
        else if(csv_file=="Water.csv"){
            curr_building.water_params = deserialize_param(serialized_param);
        }
        else if(csv_file=="Electricity.csv"){
            curr_building.elec_params = deserialize_param(serialized_param);
        }
        wait(NULL); 
    }


    std::string pipe_name = "/tmp/bill_pipe_" + std::to_string(id);
    mkfifo(pipe_name.c_str(), 0666);
   


     LOG(INFO,"named pipe created.");
    int named_pipe_fd = open(pipe_name.c_str(), O_WRONLY);
    if (named_pipe_fd == -1) {
        LOG(ERROR,"Opening of named pipe failed.");
        return 1;
    }

    std::string serialized_building = serialize_building(curr_building);

    write(named_pipe_fd, serialized_building.c_str(), serialized_building.size() + 1);
    close(named_pipe_fd);



    return 0;
}


