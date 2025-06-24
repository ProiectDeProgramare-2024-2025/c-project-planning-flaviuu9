#ifndef ORDER_H
#define ORDER_H

#include "Vehicle.h"
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class Order {
public:
    int day, month, year;
    std::vector<Vehicle> vehicles;

    Order();
    Order(int d, int m, int y);
    Order(std::vector<Vehicle> vehicles, Order date);

    static Order today();
    static void loadOrders(const std::string& filename);
    static void displayAllOrders();

    std::string toString() const;
    bool saveToFile(const std::string& filename) const;
    void display() const;

    std::vector<Vehicle> getVehicles() const;

    // JSON support
    friend void from_json(const nlohmann::json& j, Order& o);
    friend void to_json(nlohmann::json& j, const Order& o);
};

#endif // ORDER_H