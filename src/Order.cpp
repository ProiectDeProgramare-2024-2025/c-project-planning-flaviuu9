#include "../include/Order.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../include/colors.h"

using json = nlohmann::json;

Order::Order() : day(1), month(1), year(2000) {}

Order::Order(int d, int m, int y) : day(d), month(m), year(y) {}

Order::Order(std::vector<Vehicle> v, Order d) : vehicles(std::move(v)), day(d.day), month(d.month), year(d.year) {}

Order Order::today() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    return Order(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

std::string Order::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << day << "/"
        << std::setfill('0') << std::setw(2) << month << "/"
        << year;
    return oss.str();
}

void Order::display() const {
    std::cout << Colors::highlight("📦 Order Date: ") << Colors::info("\"" + toString() + "\"") << "\n";
    std::cout << Colors::info("--------------------------------------------------\n");
    double total = 0;
    for (const auto& v : vehicles) {
        std::cout << "Product: " << v.getBrand() << " " << v.getModel()
                  << " | Chassis: " << v.getChassis() << "\n"
                  << "Price: $" << std::fixed << std::setprecision(2) << v.getPrice() << "\n";
        std::cout << Colors::info("--------------------------------------------------\n");
        total += v.getPrice();
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << total;
    std::cout << Colors::success("Order Total: $" + oss.str()) << "\n\n";
}

bool Order::saveToFile(const std::string& filename) const {
    json jOrder;
    jOrder["date"] = toString();
    jOrder["vehicles"] = vehicles;

    json allOrders = json::array();
    std::ifstream inFile(filename);
    if (inFile) {
        try {
            inFile >> allOrders;
        } catch (...) {
            allOrders = json::array(); // în caz de fișier gol sau corupt
        }
    }
    inFile.close();

    allOrders.push_back(jOrder);

    std::ofstream outFile(filename);
    if (!outFile) return false;
    outFile << std::setw(4) << allOrders;
    return true;
}

std::vector<Order> allOrders;

void Order::loadOrders(const std::string& filename) {
    allOrders.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << Colors::error("[ERROR] Cannot open orders file.\n");
        return;
    }

    json j;
    try {
        file >> j;
        for (const auto& entry : j) {
            if (!entry.is_object() || !entry.contains("date") || !entry.contains("vehicles")) {
                std::cerr << Colors::error("[ERROR] Skipping malformed order.\n");
                continue;
            }

            try {
                allOrders.push_back(entry.get<Order>());
            } catch (const std::exception& e) {
                std::cerr << Colors::error("[ERROR] Could not parse order: ") << e.what() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] JSON parsing failed: ") << e.what() << "\n";
    }
}

void Order::displayAllOrders() {
    std::cout << Colors::title("\n========= All Orders =========\n");
    if (allOrders.empty()) {
        std::cout << Colors::warning("No orders found.\n");
        return;
    }

    for (const auto& order : allOrders) {
        order.display();
    }
}

std::vector<Vehicle> Order::getVehicles() const {
    return vehicles;
}

void from_json(const json& j, Order& o) {
    std::string dateStr = j.at("date").get<std::string>();
    sscanf(dateStr.c_str(), "%d/%d/%d", &o.day, &o.month, &o.year);
    o.vehicles = j.at("vehicles").get<std::vector<Vehicle>>();
}

void to_json(json& j, const Order& o) {
    j = json{
        {"date", o.toString()},
        {"vehicles", o.vehicles}
    };
}