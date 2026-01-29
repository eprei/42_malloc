cmake_minimum_required(VERSION 3.20)
project(malloc C)

set(CMAKE_C_STANDARD 11)

# Definir HOSTTYPE
if(NOT DEFINED ENV{HOSTTYPE})
    execute_process(
            COMMAND uname -m
            OUTPUT_VARIABLE UNAME_M
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
            COMMAND uname -s
            OUTPUT_VARIABLE UNAME_S
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(HOSTTYPE "${UNAME_M}_${UNAME_S}")
else()
    set(HOSTTYPE $ENV{HOSTTYPE})
endif()

set(LIBNAME "libft_malloc_${HOSTTYPE}.so")

# Compilar libft
add_subdirectory(libft)

# Biblioteca compartida malloc
add_library(ft_malloc SHARED
        src/malloc.c
        src/free.c
        src/realloc.c
        src/show_alloc_mem.c
        src/utils.c
)

set_target_properties(ft_malloc PROPERTIES
        OUTPUT_NAME "ft_malloc_${HOSTTYPE}"
        PREFIX "lib"
        SUFFIX ".so"
        COMPILE_FLAGS "-fPIC"
        LINK_FLAGS "-flat_namespace -undefined dynamic_lookup"
)

target_link_libraries(ft_malloc ft)

# Ejecutable de prueba
add_executable(test_malloc test/main.c)
target_link_options(test_malloc PRIVATE
        -flat_namespace
        -undefined dynamic_lookup
)

# Custom target para ejecutar con run.sh
add_custom_target(run_test
        COMMAND ${CMAKE_SOURCE_DIR}/run.sh ${CMAKE_BINARY_DIR}/test_malloc
        DEPENDS test_malloc ft_malloc
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)