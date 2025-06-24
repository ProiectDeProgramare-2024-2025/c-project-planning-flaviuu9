#ifndef STOCK_H
#define STOCK_H

#include "Vehicle.h"
#include <vector>
#include <string>

class Stock {
private:
    std::vector<Vehicle> vehicles;

public:
    bool loadFromFile(const std::string& filename);

    bool saveToFile(const std::string& filename) const;

    bool addVehicle(const Vehicle& v);

    bool deleteVehicle(const std::string& chassis);

    bool modifyPrice(const std::string& chassis, double newPrice);

    bool exportToFile(const std::string& filename) const;

    // Find a vehicle by chassis
    Vehicle* findVehicle(const std::string& chassis);
    const Vehicle* findVehicle(const std::string& chassis) const;

    void displayAll() const;

    // Get reference to vehicle list (for order system)
    const std::vector<Vehicle>& getAllVehicles() const;

    std::vector<Vehicle> search(const std::string& keyword) const;
};

#endif