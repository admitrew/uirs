cmake_minimum_required(VERSION 3.16)

project(therion_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ВАЖНО: Qt6 предпочтительнее, но Qt5 тоже подойдёт
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(therion_preview
    src/main.cpp
    src/ui/MapScene.cpp
    src/ui/LineItem.cpp
    src/ui/PointItem.cpp
    src/parser/Th2Parser.cpp
    src/ui/StyleManager.cpp
    src/core/TherionTypes.cpp
    src/parser/Th2Writer.cpp

)

target_link_libraries(therion_preview
    Qt6::Widgets
)

target_include_directories(therion_preview PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
