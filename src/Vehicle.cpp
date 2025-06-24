#include "../include/Vehicle.h"
#include <iomanip>
#include <nlohmann/json.hpp>
#include <iostream>
#include "../include/colors.h"

using json = nlohmann::json;

Vehicle::Vehicle() {}

Vehicle::Vehicle(const std::string& brand, const std::string& model, const std::string& chassis, double price)
    : brand(brand), model(model), chassis(chassis), price(price) {}

std::string Vehicle::getChassis() const { return chassis; }
std::string Vehicle::getBrand() const { return brand; }
std::string Vehicle::getModel() const { return model; }
double Vehicle::getPrice() const { return price; }

void Vehicle::setPrice(double p) { price = p; }
void Vehicle::setBrand(const std::string& newBrand) { brand = newBrand; }
void Vehicle::setModel(const std::string& newModel) { model = newModel; }

void Vehicle::display() const {
    std::cout << std::left << std::setw(10) << brand
              << std::setw(10) << model
              << std::setw(15) << chassis
              << std::fixed << std::setprecision(2)
              << "$" << std::setw(9) << price << "\n";
}

bool Vehicle::operator==(const Vehicle& other) const {
    return chassis == other.chassis;
}

// JSON serialization
void to_json(json& j, const Vehicle& v) {
    j = json{
        {"chassis", v.chassis},
        {"brand", v.brand},
        {"model", v.model},
        {"price", v.price}
    };
}

// JSON deserialization with error handling
void from_json(const json& j, Vehicle& v) {
    try {
        if (!j.contains("chassis") || !j.contains("brand") ||
            !j.contains("model") || !j.contains("price")) {
            throw std::invalid_argument("Missing one or more required fields in JSON.");
        }

        j.at("chassis").get_to(v.chassis);
        j.at("brand").get_to(v.brand);
        j.at("model").get_to(v.model);
        j.at("price").get_to(v.price);
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] Vehicle JSON deserialization failed: ") << e.what() << "\n";
        v = Vehicle();
    }
}