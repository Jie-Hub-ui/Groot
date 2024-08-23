# 设置 cmake 版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 设置项目名
PROJECT(test)
MESSAGE("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}") # 打印项目名称

# 设置编译器版本 c++11
#SET(CMAKE_CXX_STANDARD 11)
ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-DAUTO_RUN)

# 设置源码路径
SET(SOURCE_ROOT_DIR "/workspace/Groot/Groot/src/basecore")
MESSAGE("SOURCE_ROOT_DIR: ${SOURCE_ROOT_DIR}")

# 设置需要编译的源码目录
AUX_SOURCE_DIRECTORY(${SOURCE_ROOT_DIR}/src/app  SOURCE_APP_DIR)
AUX_SOURCE_DIRECTORY(${SOURCE_ROOT_DIR}/src/text SOURCE_TEXT_DIR)
AUX_SOURCE_DIRECTORY(${SOURCE_ROOT_DIR}/src/lang SOURCE_LANG_DIR)

# 设置需要引用的头文件目录
INCLUDE_DIRECTORIES( ${SOURCE_ROOT_DIR}/include )

# 设置需要编译的源码
ADD_EXECUTABLE(test main.cpp 
                    ${SOURCE_APP_DIR} 
                    ${SOURCE_TEXT_DIR} 
                    ${SOURCE_LANG_DIR}
              )

# 设置目标文件生成的路径
SET(TARGET_DIR ${SOURCE_ROOT_DIR}/test/target)
SET(EXECUTABLE_OUTPUT_PATH ${TARGET_DIR})

######################################################################################################
# 拉取第三方仓库
#INCLUDE(FetchContent)
#FetchContent_Declare(	ResourceFile	                                                # 库名字
#						GIT_REPOSITORY git@github.com:Jie-Hub-ui/ResourceFile.git	    # 仓库地址
#						GIT_TAG master                                                  # 库版本
#						SOURCE_DIR ./ResourceFile                                       # 指定库下载地址
#					)
#FetchContent_MakeAvailable(ResourceFile)
######################################################################################################

# 编译
# cd build
#   cmake -G "MinGW Makefiles" ../
#   make
# 或者
# cmake -B build_path -S src_path
#   build_path: 构建目录
#   src_path：源码目录，即 CMakeLists.txt 目录
# make build_path