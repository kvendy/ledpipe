cmake_minimum_required (VERSION 3.1)

project ("ledpipe")

find_package(Threads REQUIRED)

include_directories("include/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(SRC_LIST "src/main.cpp" "src/device.cpp" "src/logic.cpp")

if (WIN32)
    set(SRC_LIST ${SRC_LIST}  "src/pipe_windows.cpp")
endif (WIN32)
if (UNIX)
    set(SRC_LIST ${SRC_LIST}  "src/pipe_unix.cpp")
endif (UNIX)

add_executable (${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Threads::Threads)
