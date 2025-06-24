#include "../include/Stock.h"
#include "../include/colors.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

bool Stock::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << Colors::error("Failed to open stock file: " + filename + "\n");
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cerr << Colors::error(std::string("Error parsing stock JSON: ") + e.what() + "\n");
        return false;
    }

    vehicles.clear();
    try {
        for (const auto& item : j) {
            vehicles.push_back(item.get<Vehicle>());
        }
    } catch (const std::exception& e) {
        std::cerr << Colors::error(std::string("Error converting JSON to Vehicle: ") + e.what() + "\n");
        return false;
    }

    return true;
}

bool Stock::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << Colors::error("Failed to open file for saving: " + filename + "\n");
        return false;
    }

    try {
        json j = vehicles;
        file << j.dump(4);
    } catch (const std::exception& e) {
        std::cerr << Colors::error(std::string("Error writing JSON to file: ") + e.what() + "\n");
        return false;
    }

    return true;
}

bool Stock::addVehicle(const Vehicle& v) {
    if (findVehicle(v.getChassis())) {
        return false; // already exists
    }
    vehicles.push_back(v);
    return true;
}

bool Stock::deleteVehicle(const std::string& chassis) {
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it) {
        if (it->getChassis() == chassis) {
            vehicles.erase(it);
            return true;
        }
    }
    return false;
}

bool Stock::modifyPrice(const std::string& chassis, double newPrice) {
    Vehicle* v = findVehicle(chassis);
    if (!v) return false;
    v->setPrice(newPrice);
    return true;
}

Vehicle* Stock::findVehicle(const std::string& chassis) {
    for (auto& v : vehicles) {
        if (v.getChassis() == chassis)
            return &v;
    }
    return nullptr;
}

const Vehicle* Stock::findVehicle(const std::string& chassis) const {
    for (const auto& v : vehicles) {
        if (v.getChassis() == chassis)
            return &v;
    }
    return nullptr;
}

void Stock::displayAll() const {
    if (vehicles.empty()) {
        std::cout << Colors::warning("No vehicles in stock.\n");
        return;
    }

    std::cout << Colors::info("");
    std::cout << std::left
              << std::setw(12) << "Brand"
              << std::setw(12) << "Model"
              << std::setw(12) << "Chassis"
              << std::setw(10) << "Price" << "\n";
    std::cout << "--------------------------------------------------\n";

    for (const auto& v : vehicles) {
        std::cout << std::left
                  << std::setw(12) << v.getBrand()
                  << std::setw(12) << v.getModel()
                  << std::setw(12) << v.getChassis()
                  << "$" << std::fixed << std::setprecision(2) << v.getPrice()
                  << "\n";
    }
}

const std::vector<Vehicle>& Stock::getAllVehicles() const {
    return vehicles;
}

std::vector<Vehicle> Stock::search(const std::string& keyword) const {
    std::vector<Vehicle> results;
    for (const auto& v : vehicles) {
        if (v.getBrand().find(keyword) != std::string::npos ||
            v.getModel().find(keyword) != std::string::npos ||
            v.getChassis().find(keyword) != std::string::npos) {
            results.push_back(v);
            }
    }
    return results;
}

bool Stock::exportToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    out << "Brand,Model,Chassis,Price\n";
    for (const auto& v : vehicles) {
        out << v.getBrand() << ","
            << v.getModel() << ","
            << v.getChassis() << ","
            << std::fixed << std::setprecision(2) << v.getPrice()
            << "\n";
    }

    out.close();
    return true;
}