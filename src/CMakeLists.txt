
add_subdirectory(core)

add_subdirectory(ui)
# target_link_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core})

add_executable(OTP-Client main.cpp "core/include/hmac.h" "core/include/hash.h" "core/include/encode.h")

# target_link_directories(OTP-Client PRIVATE crypto_lib digit_convert)
target_link_libraries(OTP-Client PRIVATE crypto_lib)
# target_link_libraries(OTP-Client PRIVATE digit_convert)

target_include_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include)