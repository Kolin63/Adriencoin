# Minimum CMake version required, we'll just use the latest version.
cmake_minimum_required(VERSION 3.22)
# Project name, version and description
project(discord-bot VERSION 1.0 DESCRIPTION "A discord bot")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(DEBUG "ON for Debug, OFF for Release" OFF)
message(STATUS "The DEBUG status is ${DEBUG}. Configure it with `cmake -DDEBUG=<ON|OFF> ..` when in build/")
 
# Create an executable
add_executable(${PROJECT_NAME}
    src/Random.h
    src/botToken.h
    src/cache.cpp
    src/cache.h
    src/daily.cpp
    src/daily.h
    src/inventory.h
    src/item.h
    src/job.cpp
    src/job.h
    src/leaderboard.cpp
    src/leaderboard.h
    src/main.cpp
    src/onEvent.cpp
    src/onEvent.h
    src/onReady.cpp
    src/onReady.h
    src/player.cpp
    src/player.h
    src/product.cpp
    src/product.h
    src/shop.cpp
    src/shop.h
    src/stock.cpp
    src/stock.h
    src/trade.cpp
    src/trade.h
    src/util.h
    src/emoji.h
    src/dungeon.cpp
    src/dungeon.h
    src/attributes.cpp
    src/attributes.h
    src/statistics.cpp
    src/statistics.h
    src/role.h
    src/bank.cpp
    src/bank.h
    src/coop.cpp
    src/coop.h 
)
 
# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(DPP REQUIRED)
 
# Link the pre-installed DPP package.
target_link_libraries(${PROJECT_NAME} 
    ${DPP_LIBRARIES}
)
 
# Include the DPP directories.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${DPP_INCLUDE_DIR}
)
 
# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Set Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")

if (DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif()
