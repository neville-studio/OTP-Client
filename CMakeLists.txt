# Author: Neville Studio
# This is a global CMakeLists.txt file for the project
#
cmake_minimum_required (VERSION 3.8)

# Project name
project(OTP-Client)

# Set the Cpp stanard
set(CMAKE_CXX_STANDARD 17)

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_subdirectory(src)
# Add the crypto library
