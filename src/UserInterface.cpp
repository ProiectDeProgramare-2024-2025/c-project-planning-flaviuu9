#include "../include/UserInterface.h"
#include "../include/colors.h"
#include <iostream>
#include <iomanip>
#include <exception>

using namespace std;

void UserInterface::showAdminHelp() {
    try {
        std::cout << Colors::title("\n================ ADMIN MENU ==================\n");
        std::cout << Colors::info("view_stock") << "                                        - Show all vehicles in stock\n";
        std::cout << Colors::info("view_orders") << "                                       - Show all orders\n";
        std::cout << Colors::info("search_vehicle <keyword>") << "                          - Search vehicle by brand/model/chassis\n";
        std::cout << Colors::info("add_vehicle <chassis> <brand> <model> <price>") << "     - Add new vehicle to stock\n";
        std::cout << Colors::info("delete_vehicle <chassis>") << "                          - Delete vehicle by chassis number\n";
        std::cout << Colors::info("modify_vehicle <chassis> <new_price>") << "              - Modify price of a vehicle\n";
        std::cout << Colors::info("help") << "                                              - Show this help menu again\n";
        std::cout << Colors::title("===============================================\n");
    } catch (const std::exception& e) {
        std::cerr << Colors::error("[ERROR] Failed to display admin help: ") << e.what() << "\n";
    }
}

void UserInterface::showUserMenu() {
    try {
        std::cout << Colors::title("\n================================= USER MENU =================================\n");
        std::cout << Colors::info("view_stock") << "                                        - Show all vehicles in stock\n";
        std::cout << Colors::info("add_to_cart <chassis>") << "                             - Add a vehicle to your cart\n";
        std::cout << Colors::info("remove_from_cart <chassis>") << "                        - Remove a vehicle from your cart\n";
        std::cout << Colors::info("view_cart") << "                                         - View your shopping cart\n";
        std::cout << Colors::info("place_order") << "                                       - Finalize and place your order\n";
        std::cout << Colors::info("add_vehicle <brand> <model> <chassis> <price>") << "     - List a vehicle for selling\n";
        std::cout << Colors::info("search_vehicle <keyword>") << "                          - Search vehicle by brand or model\n";
        std::cout << Colors::info("sort_price") << "                                        - Sort vehicles by price\n";
        std::cout << Colors::info("filter_price <min> <max>") << "                          - Filter vehicles by price range\n";
        std::cout << Colors::info("help") << "                                              - Show this help menu again\n";
        std::cout << Colors::title("===============================================================================\n");
    } catch (const exception& e) {
        cerr << Colors::error("[ERROR] Failed to display user help: ") << e.what() << "\n";
    }
}

void UserInterface::showWelcome(const std::string& appName, const std::string& stockFile, const std::string& ordersFile) {
    try {
        std::cout << Colors::highlight("==================================================\n");
        std::cout << Colors::highlight("   Welcome to " + appName + " System") << "\n";
        std::cout << Colors::highlight("==================================================\n");
        std::cout << Colors::info("[INFO] ") << "Stock file: " << stockFile << "\n";
        std::cout << Colors::info("[INFO] ") << "Orders file: " << ordersFile << "\n";
    } catch (const exception& e) {
        cerr << Colors::error("[ERROR] Failed to display welcome message: ") << e.what() << "\n";
    }
}

void UserInterface::showError(const string& message) {
    cerr << Colors::error("[ERROR] ") << message << endl;
}

void UserInterface::showSuccess(const string& message) {
    cout << Colors::success("[OK] ") << message << endl;
}

void UserInterface::showInfo(const string& message) {
    cout << Colors::warning("[INFO] ") << message << endl;
}

void UserInterface::showCart(const std::vector<Vehicle>& cart) {
    try {
        cout << Colors::title("\n=================== YOUR CART ===================\n");
        if (cart.empty()) {
            cout << Colors::warning("Cart is empty.\n");
        } else {
            for (const auto& v : cart) {
                v.display();
            }
        }
        cout << Colors::title("=================================================\n");
    } catch (const exception& e) {
        cerr << Colors::error("[ERROR] Failed to display cart: ") << e.what() << endl;
    }
}

void UserInterface::showSearchResults(const std::vector<Vehicle>& results) {
    try {
        std::cout << Colors::title("\n=============================== Search Results ================================\n");
        if (results.empty()) {
            std::cout << Colors::warning("No matching vehicles found.\n");
            return;
        }

        for (const auto& v : results) {
            std::cout << v.getBrand() << " " << v.getModel()
                      << " | Chassis: " << v.getChassis()
                      << " | Price: $" << std::fixed << std::setprecision(2)
                      << v.getPrice() << "\n";
        }
    } catch (const exception& e) {
        cerr << Colors::error("[ERROR] Failed to display search results: ") << e.what() << endl;
    }
}