 #!/bin/sh
 export DYLD_LIBRARY_PATH=.
 export DYLD_INSERT_LIBRARIES="libft_malloc.so"
 export DYLD_FORCE_FLAT_NAMESPACE=1
 $@

# TODO averiguar que son todas estas variables de entorno
# DYLD_LIBRARY_PATH: This environment variable specifies a list of directories that the dynamic linker should search for shared libraries before searching the default locations. By setting this variable, you can control where the system looks for dynamic libraries when running an application.
# DYLD_INSERT_LIBRARIES: This environment variable allows you to specify one or more dynamic libraries that should be loaded into the address space of a process when it starts. This can be useful for injecting custom functionality or overriding existing functions in the target application.
# DYLD_FORCE_FLAT_NAMESPACE: This environment variable forces the dynamic linker to use a flat
# namespace when resolving symbols. In a flat namespace, all symbols are placed in a single global namespace, which can help avoid symbol conflicts that may arise when multiple libraries define the same symbol.
# $@: This is a special shell variable that represents all the arguments passed to the script. When you use "$@", it expands to a list of all the arguments, preserving any spaces or special characters within each argument. This is useful for passing all the arguments received by the script to another command or program.

# TODO conseguir los tests

# TODO Testear con un malloc normal y free para ver como funcionan usando strace, para ver si libera la estructura como yo lo hago cuando no hay nada

# Implementacion de malloc in c https://www.youtube.com/watch?v=ppnnIZtdy28
# Codigo c pagina 202 8.7 Example - A Storage Allocator https://www.cimat.mx/ciencia_para_jovenes/bachillerato/libros/%5BKernighan-Ritchie%5DThe_C_Programming_Language.pdf