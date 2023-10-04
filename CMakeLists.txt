cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)


project("pulse3"
    LANGUAGES CXX)
    
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})
find_package(fmt REQUIRED HINTS $ENV{FMT_ROOT})

add_link_options("-fuse-ld=mold")
add_compile_definitions("VL_DEBUG=1")
add_compile_options("-fsanitize=address,undefined")
add_compile_options("-ggdb3")
add_link_options("-fsanitize=address,undefined")


add_executable("pulsegen")

target_sources(pulsegen PRIVATE pulsegen.cxx)

verilate(pulsegen
    SOURCES "pulsegen.v" "div.v"
    PREFIX "pulsegen"
    THREADS 8
    COVERAGE
    OPT_GLOBAL "-O3 -march=native"
    TRACE
    VERILATOR_ARGS -j 8 --timing --assert --clk "clk" -Wpedantic --default-language "1364-2005")

target_precompile_headers(pulsegen PRIVATE 
    <memory>
    <verilated.h>
    <fmt/color.h>
    <verilated_vcd_c.h>)

target_link_libraries(pulsegen PRIVATE fmt)
