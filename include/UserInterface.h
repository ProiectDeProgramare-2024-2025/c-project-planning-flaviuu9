#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include <vector>
#include "Vehicle.h"

namespace UserInterface {

    void showAdminMenu();

    void showUserMenu();

    void showWelcome(const std::string& appName, const std::string& stockFile, const std::string& ordersFile);

    void showError(const std::string& message);

    void showSuccess(const std::string& message);

    void showInfo(const std::string& message);

    void showCart(const std::vector<Vehicle>& cart);

    void showSearchResults(const std::vector<Vehicle>& results);

    void showAdminHelp();

    void showUserHelp();
}

#endif