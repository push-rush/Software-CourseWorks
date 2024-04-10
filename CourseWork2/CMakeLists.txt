cmake_minimum_required(VERSION 3.0)
project(CourseWork2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-finput-charset=UTF-8")  
set(CMAKE_C_FLAGS "-finput-charset=UTF-8")  
set (CMAKE_EXE_LINKER_FLAGS "-static")

enable_testing()
add_subdirectory(${PROJECT_SOURCE_DIR}/Externals/googletest-release-1.8.1)
include_directories(${PROJECT_SOURCE_DIR}/Externals/googletest-release-1.8.1/googletest/include)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(MaxSubArray_LIB
    ${PROJECT_SOURCE_DIR}/src/MaxSubArray.cpp
)

# 生成可执行文件
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/main.cpp
)

# 链接操作
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    MaxSubArray_LIB
    gtest_main
)