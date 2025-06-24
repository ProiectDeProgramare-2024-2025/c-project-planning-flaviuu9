#include "include/Stock.h"
#include "include/UserInterface.h"
#include "include/Utils.h"
#include "include/colors.h"
#include "Order.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>

using json = nlohmann::json;

bool isValidChassis(const std::string& chassis) {
    return std::regex_match(chassis, std::regex("^[A-Z0-9]{3,}$"));
}

bool isValidName(const std::string& name) {
    return std::regex_match(name, std::regex("^[A-Za-z0-9_-]+$"));
}

int main(int argc, char* argv[]) {
    const std::string stockFile = "../data/vehicles.json";
    const std::string ordersFile = "../data/orders.json";

    UserInterface::showWelcome("Admin Vehicle Management", stockFile, ordersFile);

    Stock stock;
    if (!stock.loadFromFile(stockFile)) {
        std::cerr << Colors::error("[ERROR] Could not load stock.\n");
        return 1;
    }

    if (argc < 2) {
        UserInterface::showAdminHelp();
        return 0;
    }

    std::string command = argv[1];

    if (command == "view_stock") {
        std::cout << Colors::title("================ VEHICLES IN STOCK ================\n");
        stock.displayAll();
    }

    else if (command == "add_vehicle" && argc == 6) {
        std::string chassis = argv[2];
        std::string brand = argv[3];
        std::string model = argv[4];
        std::string priceStr = argv[5];

        if (!isValidChassis(chassis)) {
            std::cerr << Colors::error("[ERROR] Chassis must contain only uppercase letters and digits (min 3 chars).\n");
            return 1;
        }

        if (!isValidName(brand) || !isValidName(model)) {
            std::cerr << Colors::error("[ERROR] Brand and model must not contain spaces or special characters.\n");
            return 1;
        }

        if (!Utils::isDouble(priceStr) || std::stod(priceStr) < 0) {
            std::cerr << Colors::error("[ERROR] Price must be a valid positive number.\n");
            return 1;
        }

        Vehicle v(brand, model, chassis, std::stod(priceStr));
        if (stock.addVehicle(v)) {
            std::cout << Colors::success("[SUCCESS] Vehicle added.\n");
            stock.saveToFile(stockFile);
        } else {
            std::cerr << Colors::error("[ERROR] Vehicle already exists.\n");
        }
    }

    else if (command == "delete_vehicle" && argc == 3) {
        std::string chassis = argv[2];
        if (!isValidChassis(chassis)) {
            std::cerr << Colors::error("[ERROR] Invalid chassis format.\n");
            return 1;
        }
        if (stock.deleteVehicle(chassis)) {
            std::cout << Colors::success("[SUCCESS] Vehicle deleted.\n");
            stock.saveToFile(stockFile);
        } else {
            std::cerr << Colors::error("[ERROR] Vehicle not found.\n");
        }
    }

    else if (command == "modify_vehicle" && argc == 4) {
        std::string chassis = argv[2];
        std::string priceStr = argv[3];

        if (!isValidChassis(chassis)) {
            std::cerr << Colors::error("[ERROR] Invalid chassis format.\n");
            return 1;
        }

        if (!Utils::isDouble(priceStr) || std::stod(priceStr) < 0) {
            std::cerr << Colors::error("[ERROR] Price must be a valid positive number.\n");
            return 1;
        }

        if (stock.modifyPrice(chassis, std::stod(priceStr))) {
            std::cout << Colors::success("[SUCCESS] Price updated.\n");
            stock.saveToFile(stockFile);
        } else {
            std::cerr << Colors::error("[ERROR] Vehicle not found or update failed.\n");
        }
    }

    else if (command == "search_vehicle" && argc == 3) {
        std::string keyword = argv[2];
        if (keyword.empty()) {
            std::cerr << Colors::error("[ERROR] Search keyword cannot be empty.\n");
            return 1;
        }
        std::vector<Vehicle> results = stock.search(keyword);

        if (results.empty()) {
            std::cout << Colors::info("[INFO] No vehicles matched your search.\n");
        } else {
            std::cout << Colors::highlight("\n=== SEARCH RESULTS ===\n");
            for (const auto& v : results) {
                v.display();
                std::cout << Colors::title("-------------------------------------------------\n");
            }
        }
    }

    else if (command == "view_orders") {
        std::cout << Colors::title("\n================== ORDER LIST ==================\n");
        Order::loadOrders(ordersFile);
        Order::displayAllOrders();
    }

    else if (command == "help") {
        UserInterface::showAdminHelp();
    }

    else {
        std::cerr << Colors::error("[ERROR] Unknown command.\n");
        UserInterface::showAdminHelp();
    }

    return 0;
}
