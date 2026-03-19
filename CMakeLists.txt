# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.5)

# Name the project. This will be the name of the final executable.
project(MyVimShell)

# Specify the C++ standard to use (C++17 is a modern choice).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# These are crucial for Qt projects. They automatically handle Qt's
# special pre-processing steps (meta-object compiler, user interface compiler, etc.).
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt6 package on your system and specify the components we need:
# Core: Fundamental non-GUI classes.
# Gui: Base classes for GUI programming.
# Widgets: Classes for creating classic desktop UI elements.
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Define the executable target and list all the source files needed to build it.
add_executable(MyVimShell
  main.cpp
  MainWindow.h
  MainWindow.cpp
)

# Link your executable against the necessary Qt libraries.
target_link_libraries(MyVimShell Qt6::Core Qt6::Gui Qt6::Widgets)