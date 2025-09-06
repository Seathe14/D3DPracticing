# List of all C++ sources in the Benchmark application
set(Benchmark_SOURCES
    Benchmark.cpp
)

add_executable(Benchmark ${Benchmark_SOURCES})

target_include_directories(Benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/D3DPracticing
    ${CMAKE_SOURCE_DIR}/libs/benchmark/include
)

target_compile_definitions(Benchmark PRIVATE BENCHMARK_STATIC_DEFINE)

if(WIN32)
    target_link_directories(Benchmark PRIVATE ${CMAKE_SOURCE_DIR}/libs/benchmark/lib/Release)
    target_link_libraries(Benchmark PRIVATE benchmark d3d11 dxguid d3dcompiler Shlwapi)
endif()
