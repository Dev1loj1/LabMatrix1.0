cmake_minimum_required(VERSION 3.20)
project(LabMatrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(TVector STATIC TVector/src/TVector.cpp)
target_include_directories(TVector PUBLIC TVector/include)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(test_tvector TVector/tests/test_tvector.cpp)
    target_link_libraries(test_tvector PRIVATE TVector GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(test_tvector DISCOVERY_MODE PRE_TEST)
endif()
