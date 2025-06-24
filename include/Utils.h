#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "Vehicle.h"

namespace Utils {

    bool isInteger(const std::string& str);

    bool isDouble(const std::string& str);

    bool isFloat(const std::string& str);

    // Split a string by a given delimiter
    std::vector<std::string> split(const std::string& str, char delimiter);

    // Trim leading and trailing whitespace from a string
    std::string trim(const std::string& str);

    // Read all lines from a file into a vector of strings
    std::vector<std::string> readLines(const std::string& filename);

    // Write all lines from a vector of strings into a file
    void writeLines(const std::string& filename, const std::vector<std::string>& lines);

    bool saveCartToFile(const std::vector<Vehicle>& cart, const std::string& filename);

    bool loadCartFromFile(std::vector<Vehicle>& cart, const std::string& filename);
}

#endif // UTILS_H