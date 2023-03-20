
cmake_minimum_required(VERSION 3.18.1)

project("learn")

# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/freetypeInclude)

# 动态链接库或静态链接库的搜索路径
link_directories(${CMAKE_SOURCE_DIR}/../libs/${CMAKE_ANDROID_ARCH_ABI})

#找到包含所有的cpp文件
file(GLOB allCpp *.h *.cpp **/**.h **/**.cpp **/**/**.h **/**/**.cpp  **/**/**/**.h **/**/**/**.cpp  **/**/**/**/**.cpp)

add_library( # Sets the name of the library.
        # 库名称
        learn

        SHARED

        ${allCpp})

target_link_libraries(
        learn
        # 引入egl
        egl
        # 引入gles 3
        GLESv3
        # 安卓相关库
        android
        # bitmap相关
        jnigraphics
        # 安卓log
        log
        # 字体库
        freetype)