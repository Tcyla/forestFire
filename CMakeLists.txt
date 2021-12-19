cmake_minimum_required(VERSION 3.10)

# Linux Only
#

# set the project name
project(forestFire)

# finding c++ threading lib
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# finding SDL2 package
find_package(SDL2 REQUIRED)
# adding SDL2 to the project
include_directories(${SDL2_INCLUDE_DIRS})

# adding the headers directory
include_directories(headers)

# add the executable
add_executable(forestFire main.cpp Game.cpp View.cpp ForestFire.cpp StateMatrix.cpp)
target_link_libraries(forestFire ${SDL2_LIBRARIES} Threads::Threads)
