add_library(digit_convert base32.cpp base64.cpp hexString.cpp)

target_include_directories(digit_convert 
   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
   PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)

