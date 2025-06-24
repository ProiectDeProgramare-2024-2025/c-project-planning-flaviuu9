#include "../include/Utils.h"
#include "../include/colors.h"
#include "../include/Vehicle.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Check if the string represents an integer
bool Utils::isInteger(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c) && c != '-') return false;
    }
    return true;
}

// Check if the string represents a double
bool Utils::isDouble(const std::string& str) {
    std::istringstream iss(str);
    double val;
    char c;
    return iss >> val && !(iss >> c);
}

// Split a string into parts by a delimiter
std::vector<std::string> Utils::split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    try {
        std::istringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            parts.push_back(item);
        }
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] split failed: ") << e.what() << "\n";
    }
    return parts;
}

// Trim spaces from both ends
std::string Utils::trim(const std::string& str) {
    try {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] trim failed: ") << e.what() << "\n";
        return str;
    }
}

// Read file content into a vector of lines
std::vector<std::string> Utils::readLines(const std::string& filename) {
    std::vector<std::string> lines;
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << Colors::error("[ERROR] Could not open file for reading: ") << filename << "\n";
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] readLines failed: ") << e.what() << "\n";
    }
    return lines;
}

// Write a vector of lines to a file
void Utils::writeLines(const std::string& filename, const std::vector<std::string>& lines) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << Colors::error("[ERROR] Could not open file for writing: ") << filename << "\n";
            return;
        }

        for (const std::string& line : lines) {
            file << line << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] writeLines failed: ") << e.what() << "\n";
    }
}

// Check if string can be parsed to float
bool Utils::isFloat(const std::string& str) {
    try {
        std::stof(str);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool Utils::saveCartToFile(const std::vector<Vehicle>& cart, const std::string& filename) {
    try {
        std::ofstream out(filename);
        if (!out.is_open()) return false;

        json j_cart = cart;
        out << j_cart.dump(4);
        out.close();
        return true;
    } catch (...) {
        return false;
    }
}

bool Utils::loadCartFromFile(std::vector<Vehicle>& cart, const std::string& filename) {
    try {
        std::ifstream in(filename);
        if (!in.is_open()) return false;

        json j_cart;
        in >> j_cart;
        cart = j_cart.get<std::vector<Vehicle>>();
        return true;
    } catch (...) {
        return false;
    }
}
