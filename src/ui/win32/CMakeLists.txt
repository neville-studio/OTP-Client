

add_definitions(-D_UNICODE -DUNICODE)

#enable_language(RC)

    

add_library(win32form MainWinForm.cpp   "DataHandler.cpp"  )



target_link_libraries(win32form PRIVATE
            user32.lib
            gdi32.lib
            comctl32.lib
            crypt32.lib
            i18n
            global_datastore
            otp-core
)

