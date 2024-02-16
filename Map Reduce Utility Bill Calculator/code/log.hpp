#ifndef LOG_HPP
#define LOG_HPP


#include <ostream>
#include <iostream>
#include <string>
#include "colors.hpp"


const std::string ERROR = "Error";
const std::string WARNING= "Warning";
const std::string INFO = "Info";


#define LOG(level, message) logger(level, message)

void logger(const std::string& level, const std::string& message);

void logger(const std::string& level, const std::string& message) {
    std::ostream& output = (level == ERROR || level == WARNING) ? std::cerr : std::cout;
    Color color;

    if (level == ERROR) {
        color = Color::RED;
    } else if (level == WARNING) {
        color = Color::YELLOW;
    } else if (level == INFO) {
        color = Color::BLUE;
    } else {
        color = Color::DEFAULT;
    }

    output << color << level << Color::RESET << ": " << message << std::endl;
}




#endif