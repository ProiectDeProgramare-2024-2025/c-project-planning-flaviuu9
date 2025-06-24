#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Vehicle {
private:
    std::string chassis;
    std::string brand;
    std::string model;
    double price;

public:
    Vehicle();
    Vehicle(const std::string& brand, const std::string& model, const std::string& chassis, double price);

    std::string getChassis() const;
    std::string getBrand() const;
    std::string getModel() const;
    double getPrice() const;

    void setPrice(double p);
    void setBrand(const std::string& newBrand);
    void setModel(const std::string& newModel);

    // Display vehicle data in formatted form
    void display() const;

    bool operator==(const Vehicle& other) const;

    // JSON support
    friend void to_json(json& j, const Vehicle& v);
    friend void from_json(const json& j, Vehicle& v);
};

#endif