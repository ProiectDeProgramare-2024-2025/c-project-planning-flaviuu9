#include "include/Stock.h"
#include "include/UserInterface.h"
#include "include/Order.h"
#include "include/Utils.h"
#include "include/colors.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <regex>

int main(int argc, char* argv[]) {
    const std::string stockFile = "../data/vehicles.json";
    const std::string ordersFile = "../data/orders.json";
    const std::string cartFile = "../data/cart.json";

    UserInterface::showWelcome("User Vehicle Interaction", stockFile, ordersFile);

    Stock stock;
    if (!stock.loadFromFile(stockFile)) {
        UserInterface::showError("Could not load stock.");
        return 1;
    }

    static std::vector<Vehicle> cart;
    Utils::loadCartFromFile(cart, cartFile);

    if (argc < 2) {
        UserInterface::showUserMenu();
        return 0;
    }

    std::string command = argv[1];

    auto isValidChassis = [](const std::string& chassis) {
        return std::regex_match(chassis, std::regex("[A-Z0-9]{3,}"));
    };

    auto isValidText = [](const std::string& text) {
        return std::regex_match(text, std::regex("[A-Za-z0-9]+"));
    };

    if (command == "view_stock") {
        stock.displayAll();

    } else if (command == "search_vehicle" && argc == 3) {
        std::string keyword = argv[2];
        std::vector<Vehicle> results;
        for (const auto& v : stock.getAllVehicles()) {
            if (v.getBrand().find(keyword) != std::string::npos ||
                v.getModel().find(keyword) != std::string::npos) {
                results.push_back(v);
            }
        }
        if (results.empty()) {
            std::cout << Colors::error("[ERROR] No vehicles found.\n");
        } else {
            UserInterface::showSearchResults(results);
        }

    } else if (command == "sort_price") {
        std::vector<Vehicle> sorted = stock.getAllVehicles();
        std::sort(sorted.begin(), sorted.end(), [](const Vehicle& a, const Vehicle& b) {
            return a.getPrice() < b.getPrice();
        });
        std::cout << Colors::title("=== Vehicles sorted by price (ascending) ===\n");
        for (const auto& v : sorted) {
            std::cout << v.getBrand() << " " << v.getModel()
                      << " - $" << std::fixed << std::setprecision(2)
                      << v.getPrice() << "\n";
        }

    } else if (command == "filter_price" && argc == 4) {
        try {
            double minPrice = std::stod(argv[2]);
            double maxPrice = std::stod(argv[3]);

            if (minPrice > maxPrice) {
                UserInterface::showError("Minimum price cannot be greater than maximum price.");
                return 1;
            }

            std::vector<Vehicle> filtered;
            for (const auto& v : stock.getAllVehicles()) {
                if (v.getPrice() >= minPrice && v.getPrice() <= maxPrice) {
                    filtered.push_back(v);
                }
            }

            if (filtered.empty()) {
                std::cout << Colors::info("[INFO] No vehicles found in the specified price range.\n");
            } else {
                UserInterface::showSearchResults(filtered);
            }
        } catch (...) {
            UserInterface::showError("Invalid price range inputs.");
            return 1;
        }

    } else if (command == "add_vehicle" && argc == 6) {
        std::string chassis = argv[2];
        std::string brand = argv[3];
        std::string model = argv[4];
        std::string priceStr = argv[5];

        if (!isValidChassis(chassis)) {
            UserInterface::showError("Invalid chassis format. Use uppercase letters and numbers only.");
            return 1;
        }

        if (!isValidText(brand) || !isValidText(model)) {
            UserInterface::showError("Brand and model must not contain spaces or special characters.");
            return 1;
        }

        if (!Utils::isDouble(priceStr) || std::stod(priceStr) < 0) {
            UserInterface::showError("Price must be a valid positive number.");
            return 1;
        }

        if (stock.findVehicle(chassis)) {
            UserInterface::showError("A vehicle with this chassis already exists.");
            return 1;
        }

        Vehicle v(brand, model, chassis, std::stod(priceStr));
        if (stock.addVehicle(v)) {
            stock.saveToFile(stockFile);
            UserInterface::showSuccess("Vehicle added.");
        } else {
            UserInterface::showError("Failed to add vehicle.");
        }

    } else if (command == "add_to_cart" && argc == 3) {
        std::string chassis = argv[2];
        const Vehicle* v = stock.findVehicle(chassis);
        if (!v) {
            UserInterface::showError("Vehicle not found.");
            return 1;
        }
        auto exists = std::find_if(cart.begin(), cart.end(), [&](const Vehicle& ve) {
            return ve.getChassis() == chassis;
        });
        if (exists != cart.end()) {
            UserInterface::showError("Vehicle already in cart.");
            return 1;
        }
        cart.push_back(*v);
        Utils::saveCartToFile(cart, cartFile);
        UserInterface::showSuccess("Vehicle added to cart.");

    } else if (command == "remove_from_cart" && argc == 3) {
        std::string chassis = argv[2];
        auto it = std::remove_if(cart.begin(), cart.end(), [&](const Vehicle& v) {
            return v.getChassis() == chassis;
        });
        if (it != cart.end()) {
            cart.erase(it, cart.end());
            Utils::saveCartToFile(cart, cartFile);
            UserInterface::showSuccess("Vehicle removed from cart.");
        } else {
            UserInterface::showError("Vehicle not in cart.");
        }

    } else if (command == "view_cart") {
        if (cart.empty()) {
            std::cerr << Colors::error("[ERROR] Cart is empty.\n");
        } else {
            UserInterface::showCart(cart);
        }

    } else if (command == "place_order") {
        if (cart.empty()) {
            UserInterface::showError("Cart is empty.");
            return 1;
        }
        double total = 0;
        std::cout << Colors::title("\n=== ORDER SUMMARY ===\n");
        for (const auto& v : cart) {
            std::cout << v.getBrand() << " " << v.getModel()
                      << " - $" << std::fixed << std::setprecision(2)
                      << v.getPrice() << "\n";
            total += v.getPrice();
        }
        std::cout << Colors::info("Total: $") << std::fixed << std::setprecision(2) << total << "\n";

        Order order(cart, Order::today());
        order.display();
        if (order.saveToFile(ordersFile)) {
            for (const auto& v : cart) {
                stock.deleteVehicle(v.getChassis());
            }
            stock.saveToFile(stockFile);
            cart.clear();
            Utils::saveCartToFile(cart, cartFile);
            UserInterface::showSuccess("Order saved and stock updated.");
        } else {
            UserInterface::showError("Failed to save order.");
        }

    } else {
        UserInterface::showError("Unknown command.");
        UserInterface::showUserMenu();
    }

    return 0;
}
