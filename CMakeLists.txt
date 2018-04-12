# 设置工程名称
project (oled)
# 设置可执行文件名称
set(MY_TARGET oled)
# 需要链接的动态链接库
set(EXTRA_LIBS libpthread-2.19-2014.07.so )
# CMAKE最小版本
cmake_minimum_required (VERSION 2.6)

# 设置目标平台系统
set(CMAKE_SYSTEM_NAME Linux)

# 设置交叉编译库路径
set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabi/lib /usr/arm-linux-gnueabi/include /usr/local/arm/arm-linux-gnueabihf-4.9)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)#只在交叉编译库路径中寻找
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 设置交叉编译器
set(CMAKE_C_COMPILER /usr/local/arm/arm-linux-gnueabihf-4.9/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/local/arm/arm-linux-gnueabihf-4.9/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_EXTENSIONS "-pipe -g -Wall -W -fPIE")
set(CMAKE_CXX_EXTENSIONS "-pipe -g -Wall -W -fPIE")
#设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

message("++++++++++++++Start Build+++++++++++++++++")

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/HARDWARE/inc 
                    ${PROJECT_SOURCE_DIR}/COMMON/inc 
                    ${PROJECT_SOURCE_DIR}/LIB/inc )

# 添加源文件目录
aux_source_directory(${PROJECT_SOURCE_DIR}/HARDWARE/src HARDWARESRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/COMMON/src COMMONSRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/USER USRSRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/LIB/src LIBSRC)

# 添加子目录 子目录里面放一些别的编译好的模块
#ADD_SUBDIRECTORY(src)

# 链接库搜索路径
link_directories(/usr/local/arm/arm-linux-gnueabihf-4.9/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf/)

# 添加动态库
link_libraries(${EXTRA_LIBS})

# 添加可执行文件（可执行文件名 [配置] 源文件）
add_executable(${MY_TARGET} ${USRSRC} ${HARDWARESRC} ${COMMONSRC} ${LIBSRC})

# 执行文件链接属性
TARGET_LINK_LIBRARIES(${MY_TARGET} ${EXTRA_LIBS})


