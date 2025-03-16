add_library(i18n i18n.cpp)

target_include_directories(i18n
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)

