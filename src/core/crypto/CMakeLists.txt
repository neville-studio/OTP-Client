# add all files in the current directory
# file(GLOB OTP_SOURCES "*.cpp" "*.h")

add_library(crypto_lib sha1.cpp hmac.cpp)

target_include_directories(crypto_lib 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)

target_link_libraries(crypto_lib PRIVATE digit_convert)





