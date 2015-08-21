cmake_minimum_required(VERSION 2.8.3)
project(stronghold_server CXX)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR 	${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB INCLUDES ${INCLUDE_DIR}/*.hpp)
file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)
 
add_executable(stronghold_server ${SOURCES} ${INCLUDES})



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

# Tell MSVC not to warn us about unchecked iterators in debug builds
if(${MSVC})
    set_target_properties(stronghold_server PROPERTIES
        COMPILE_DEFINITIONS_DEBUG _SCL_SECURE_NO_WARNINGS)
endif()


#### stronghold_shared ####
add_subdirectory(libs/shared)
### \stronghold_shared ####


##### boost #####
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55 REQUIRED system)
#### \boost #####


#### import ####
target_include_directories(stronghold_server PRIVATE 
    ${INCLUDE_DIR}
	${STRONGHOLD_SHARED_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
)
target_link_libraries(stronghold_server 
	stronghold_shared
	${Boost_LIBRARIES}
)
### \import ####
