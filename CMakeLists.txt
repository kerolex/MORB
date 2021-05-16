cmake_minimum_required(VERSION 2.8.9)
project(MORB)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-ignored-attributes -Wno-deprecated-declarations -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes -Wno-deprecated-declarations -O3 -march=native")

################################################################################
# Check C++11 or C++0x support

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fext-numeric-literals")
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
# Set OpenCV
set(OpenCV_DIR "/usr/local/opencv/opencv-3.4.1")
find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR} NO_DEFAULT_PATH)

if(NOT OpenCV_FOUND)
   message(FATAL_ERROR "OpenCV > 3.x not found.")
else()
	message(STATUS "OpenCV found.")
endif()


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
###############################################################

include_directories(
${PROJECT_SOURCE_DIR}
)

add_library(${PROJECT_NAME}
src/morb.cpp
src/Frame.cpp
src/utilities.cpp
src/kpsextractor.cpp
src/mainMatcher.cpp
src/matcher.cpp
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} -lboost_filesystem -lboost_system)

# Build examples
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})


#
add_executable(morb 
${PROJECT_SOURCE_DIR}/src/mainMatcher.cpp)
target_link_libraries(morb ${PROJECT_NAME})
#


#
add_executable(detector 
${PROJECT_SOURCE_DIR}/src/kpsextractor.cpp)
target_link_libraries(detector ${PROJECT_NAME})
#

