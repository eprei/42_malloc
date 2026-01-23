cmake_minimum_required(VERSION 3.10)
project(libft C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

set(SRCS
        ft_toupper.c
        ft_tolower.c
        ft_strrchr.c
        ft_strncmp.c
        ft_strlen.c
        ft_strchr.c
        ft_putstr_fd.c
        ft_putnbr_fd.c
        ft_putchar_fd.c
        ft_isprint.c
        ft_isdigit.c
        ft_isascii.c
        ft_isalpha.c
        ft_isalnum.c
        ft_bzero.c
        ft_atoi.c
        ft_putendl_fd.c
        ft_memcpy.c
        ft_memchr.c
        ft_memcmp.c
        ft_memmove.c
        ft_memset.c
        ft_calloc.c
        ft_strdup.c
        ft_strlcpy.c
        ft_strnstr.c
        ft_strjoin.c
        ft_substr.c
        ft_itoa.c
        ft_strlcat.c
        ft_striteri.c
        ft_strmapi.c
        ft_strtrim.c
        ft_split.c
        ft_printf.c
        ft_strcmp.c
)

add_library(ft STATIC ${SRCS})

target_include_directories(ft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
