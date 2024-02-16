#include "log.hpp"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include "parameters.hpp"
#include "csv.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        LOG(ERROR, "No file path provided");
        return 1;
    }
    Table table;
    read_csv(argv[1], table); 
    Parameters params = calculate_parameters(table);
    std::string serialized_params = serialize_param(params);
    write(STDOUT_FILENO, serialized_params.c_str(), serialized_params.size() + 1);  // +1 to include the null terminator
    LOG(INFO,"Serialized param is sent to building process");

    return 0;
}
