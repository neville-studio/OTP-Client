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

# file(GLOB STRING_FILES "${PROJECT_SOURCE_DIR}/res/strings/*.json")

# message("copying international files from ${PROJECT_SOURCE_DIR}/res/strings/ to ${CMAKE_BINARY_DIR}/i18n")


if(WIN32)
    
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
    set(MANIFEST ${CMAKE_SOURCE_DIR}/res/OTP-Client.exe.manifest)
    set_source_files_properties(${MANIFEST} PROPERTIES VS_DEPLOYMENT_CONTENT 0)
    
    # judge the processor architecture of the host machine (x86, x64, arm, arm64)
    #if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    #    set(PROCESSOR_A "x64")
    #elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    #    set(PROCESSOR_A "x86")
    #elseif(<CONFIG>_OUTPUT_NAME MATCHES "arm")
    #    set(PROCESSOR_A "arm")
    #elseif(<CONFIG>_OUTPUT_NAME MATCHES "arm64")
    #    set(PROCESSOR_A "arm64")
    #endif()

    #set(WINDOWS_SDK_VERSION 10.0.22621.0 CACHE INTERNAL "")
    #set(WINDOWS_SDK_ROOT "C:/Program Files (x86)/Windows Kits/10" CACHE INTERNAL "")
    #set(WINDOWS_SDK_INCLUDE_DIR "${WINDOWS_SDK_ROOT}/Include/${WINDOWS_SDK_VERSION}" CACHE INTERNAL "")
    #set(WINDOWS_SDK_LIB_DIR "${WINDOWS_SDK_ROOT}/Lib/${WINDOWS_SDK_VERSION}/${PROCESSOR_A}" CACHE INTERNAL "")
    #set(WINDOWS_APPSDK_ROOT "D:/Program Files/WINDOWS_APP_SDK_1.7" CACHE INTERNAL "")
    #set(WINDOWS_APPSDK_LIB_DIR "${WINDOWS_APPSDK_ROOT}/lib" CACHE INTERNAL "")
    #set(WINDOWS_APPSDK_INCLUDE_DIR "${WINDOWS_APPSDK_ROOT}/include" CACHE INTERNAL "")
    
    
    #else()
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-entry,_WinMain@16 ")
    #endif()
endif()

# Add the executable
add_subdirectory(src)

