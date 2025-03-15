
add_subdirectory(core)

add_subdirectory(ui)
# target_link_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core})

add_executable(OTP-Client main.cpp )

# target_link_directories(OTP-Client PRIVATE crypto_lib digit_convert)
target_link_libraries(OTP-Client PRIVATE crypto_lib sntp-client digit_convert)
# target_link_libraries(OTP-Client PRIVATE digit_convert)

target_include_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include)