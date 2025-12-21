if(WIN32)
    set(SOURCE_FILE sntp-windows.cpp)
else()
    set(SOURCE_FILE sntp-posix.cpp)
endif()
#else
#    target_link_libraries(sntp-client PUBLIC )
#    set(SOURCE_FILE sntp-posix.cpp)

add_library(sntp-client ${SOURCE_FILE})

target_include_directories(digit_convert 
   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
   PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)

if(win32)
    target_link_libraries(sntp-client PUBLIC ws2_32)
endif()