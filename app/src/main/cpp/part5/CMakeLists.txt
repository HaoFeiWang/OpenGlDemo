add_library(
        lib_part5
        SHARED
        ../utils/log.h
        ../utils/asset_utils.h
        ../utils/asset_utils.cpp
        ../utils/gl_utils.h
        ../utils/gl_utils.cpp
        ../utils/stb_image.h
        part5_jni_call.cpp
        Part5NativeRender.h
        Part5NativeRender.cpp
)
find_library(
        log-lib
        log)
target_link_libraries(
        lib_part5
        android
        GLESv3
        ${log-lib})