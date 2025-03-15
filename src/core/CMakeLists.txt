# 假设这是core模块所在目录的CMakeLists.txt

# 添加core/include目录到头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(crypto)
add_subdirectory(digit_convert)
add_subdirectory(otp)
add_subdirectory(sntp)

