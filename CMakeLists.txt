cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)

cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_PHYSICAL_CORES)
message("Using ${CORES} cores")

project("pulse3"
    LANGUAGES CXX)

    
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})
find_package(fmt REQUIRED HINTS $ENV{FMT_ROOT})
find_program(MOLD_FOUND mold)

# mold can be buggy on BSDs
if (EXISTS ${MOLD_FOUND} AND ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Mold linker enabled")
    add_link_options("-fuse-ld=mold")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message("Verilator debugging enabled")
    add_compile_definitions("VL_DEBUG=1")

    if (MSVC)
        message("ASan enabled")
        add_compile_options("/fsanitize=address")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR ${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
        message("ASan and UBSan enabled with extra debug info")
        add_compile_options("-fsanitize=address,undefined")
        add_compile_options("-ggdb3")
        add_link_options("-fsanitize=address,undefined")
    endif()
endif()

add_executable("pulsegen")

target_sources(pulsegen PRIVATE pulsegen.cxx)

verilate(pulsegen
    SOURCES "pulsegen.v" "div.v"
    PREFIX "pulsegen"
    THREADS ${CORES}
    COVERAGE
    OPT_GLOBAL "-O3 -march=native"
    TRACE
    VERILATOR_ARGS -j ${CORES} --timing --assert --clk "clk" -Wpedantic --default-language "1364-2005")

target_precompile_headers(pulsegen PRIVATE 
    <memory>
    <verilated.h>
    <fmt/color.h>
    <verilated_vcd_c.h>)

target_link_libraries(pulsegen PRIVATE fmt)
