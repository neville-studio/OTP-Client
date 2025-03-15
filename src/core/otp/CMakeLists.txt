add_library(otp-core otp.cpp)

target_include_directories(otp-core
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)

target_link_libraries(otp-core PRIVATE crypto_lib)