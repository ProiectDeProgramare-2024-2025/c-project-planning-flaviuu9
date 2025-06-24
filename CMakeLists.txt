cmake_minimum_required(VERSION 3.11)
project(VehicleSellingSystem CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable useful compiler warnings
add_compile_options(-Wall -Wextra -pedantic)

# Include header files
include_directories(include)

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Source files
set(COMMON_SOURCES
        src/Vehicle.cpp
        src/Stock.cpp
        src/Order.cpp
        src/UserInterface.cpp
        src/Utils.cpp
)

# Header files
set(COMMON_HEADERS
        include/Vehicle.h
        include/Stock.h
        include/Order.h
        include/UserInterface.h
        include/Utils.h
        include/colors.h
)

# Admin executable
add_executable(main_admin main_admin.cpp ${COMMON_SOURCES}
        include/colors.h
        include/UserInterface.h
        include/Stock.h
        include/Order.h
)
target_include_directories(main_admin PRIVATE include)
target_link_libraries(main_admin PRIVATE nlohmann_json::nlohmann_json)

# User executable
add_executable(main_user main_user.cpp ${COMMON_SOURCES}
        include/colors.h
        include/UserInterface.h
        include/Stock.h
        include/Order.h
)
target_include_directories(main_user PRIVATE include)
target_link_libraries(main_user PRIVATE nlohmann_json::nlohmann_json)