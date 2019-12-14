cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
        HUNTER_CACHE_SERVERS
        "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
        CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
        SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

project(lab-04-boost-filesystem)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
hunter_add_package(Boost COMPONENTS system filesystem)

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
#find_package(Boost CONFIG REQUIRED system filesystem)

add_library(lab-04-boost-filesystem STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp)

set(SOURCES
        sources/source.cpp
        )

set(HEADERS
        include/header.hpp
        )

add_executable(tests tests/test.cpp ${SOURCES} ${HEADERS})

target_include_directories(lab-04-boost-filesystem
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(tests GTest::main Boost::system Boost::filesystem ${Boost_LIBRARIES})

enable_testing()
add_test(NAME unit_tests COMMAND tests)

if(BUILD_COVERAGE)
  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  add_coverage(lab-04-boost-filesystem)
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()