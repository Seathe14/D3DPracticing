cmake_minimum_required(VERSION 3.20)
project(D3DPracticingSolution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The project relies on Windows-specific APIs and libraries. When configuring
# on a non-Windows host, skip adding any targets so that configuration and
# the subsequent build step complete without errors.
if(WIN32)
    add_subdirectory(D3DPracticing)
    add_subdirectory(Benchmark)
else()
    message(STATUS "Non-Windows platform detected; skipping Windows-only targets.")
endif()
