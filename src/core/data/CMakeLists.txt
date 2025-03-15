add_library(global_datastore globaldata.cpp)
target_include_directories(global_datastore 
   PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
   PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include
)