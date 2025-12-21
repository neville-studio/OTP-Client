
add_subdirectory(core)

add_subdirectory(ui)

if(WIN32)
    set(CMAKE_RC_COMPILER_INIT rc)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> /fo <OBJECT> <SOURCE>")
    message(STATUS "Source files: ${SRC_FILES}")
    message(STATUS "Resource files: ${RC_FILES}")
    SET (FILES_TOCOMPILE "main.cpp" "ui/win32/Resources.rc")
else()
    set(FILES_TOCOMPILE "main.cpp")
endif()


# target_link_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core})
add_executable(OTP-Client ${FILES_TOCOMPILE})

# target_link_directories(OTP-Client PRIVATE crypto_lib digit_convert)
target_link_libraries(OTP-Client PRIVATE crypto_lib sntp-client digit_convert win32form)
# target_link_libraries(OTP-Client PRIVATE digit_convert)

target_include_directories(OTP-Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core/include)


if(WIN32)
    # Windows系统
    if(MSVC)
        # 使用MSVC编译器时，设置预处理器定义
        add_definitions(-DUNICODE -D_UNICODE)
    endif()

    # 根据Windows版本选择不同的实现
    #if(CMAKE_SYSTEM_VERSION GREATER_EQUAL "10.0.18362")  # Windows 10, version 1903 or later
    #    message(STATUS "Detected Windows 10 or higher, using WinUI 3.")
    #    set(UI_SOURCE_FILES winui3/MainWinUI3.cpp)
    #    set(UI_HEADER_FILES winui3/MainWinUI3.h)
        # 添加对WinUI 3的支持（假设你已经安装了必要的SDK）
    #    find_package(WinUI3 REQUIRED)
    #    target_link_libraries(${PROJECT_NAME} PRIVATE WinUI3::WinUI3)
    #else()
        #message(STATUS "Detected an older version of Windows, using Win32 UI.")

        target_sources(OTP-Client PRIVATE ${MANIFEST})
        #set(UI_SOURCE_FILES ui/win32/MainWinForm.cpp)
        #set(UI_HEADER_FILES ui/win32/MainWinForm.h)
        target_link_libraries(OTP-Client PRIVATE win32form)
        # 添加对Win32的支持
    
    #endif()
else()
    message(FATAL_ERROR "Unsupported platform")
endif()


