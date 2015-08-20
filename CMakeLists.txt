# Basic CMake project
cmake_minimum_required(VERSION 2.8.11)

# Name the project after the exercise
project(stronghold_server CXX)

# Locate Boost libraries: unit_test_framework, date_time and regex
#set(BOOST_ROOT /opt/local/include/boost)
#set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55 REQUIRED system serialization)

set (EXTRA_COMPILE_FLAGS "-Wall -Werror")

include(CheckCXXCompilerFlag)
if (WIN32)
else (WIN32)
    set (EXTRA_COMPILE_FLAGS "")
    CHECK_CXX_COMPILER_FLAG(-std=c++11 HAS_CXX_11)
    if (HAS_CXX_11)
        set(EXTRA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} -std=c++11")
        message(STATUS "Enabling C++11 support")
    else(HAS_CXX_11)
        message(WARNING "No C++11 support detected, build will fail.")
    endif()
    set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}")
endif (WIN32)

# Define source files for this assignment
FILE(GLOB Sources *.cpp)
FILE(GLOB Headers *.h)
ADD_EXECUTABLE (stronghold_server 
	${Sources}
	${Headers}
	)

# We need boost includes
target_include_directories(stronghold_server PRIVATE ${Boost_INCLUDE_DIRS})

# We need boost libraries
target_link_libraries(stronghold_server ${Boost_LIBRARIES})

# Tell MSVC not to warn us about unchecked iterators in debug builds
if(${MSVC})
    set_target_properties(stronghold_server PROPERTIES
        COMPILE_DEFINITIONS_DEBUG _SCL_SECURE_NO_WARNINGS)
endif()