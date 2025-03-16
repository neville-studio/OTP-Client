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



# Add the crypto library

file(GLOB STRING_FILES "${PROJECT_SOURCE_DIR}/res/strings/*.json")

# message("copying international files from ${PROJECT_SOURCE_DIR}/res/strings/ to ${CMAKE_BINARY_DIR}/i18n")


if(WIN32)
    
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    set(MANIFEST ${CMAKE_SOURCE_DIR}/res/OTP-Client.exe.manifest)
    set_source_files_properties(${MANIFEST} PROPERTIES VS_DEPLOYMENT_CONTENT 0)
    #else()
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-entry,_WinMain@16 ")
    #endif()
endif()

# Add the executable
add_subdirectory(src)

