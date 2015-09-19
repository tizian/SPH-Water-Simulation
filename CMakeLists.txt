cmake_minimum_required(VERSION 2.6)

project(sph)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

include_directories(
	"${SFML_INCLUDE_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
)

file(GLOB_RECURSE sph_FILES
	"${PROJECT_SOURCE_DIR}/include/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(sph ${sph_FILES})

target_link_libraries(sph ${SFML_LIBRARIES})