cmake_minimum_required (VERSION 3.0)
project (qboy)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_CXX_STANDARD 14) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

add_subdirectory(src)
#enable_testing()
#add_subdirectory(tests)
