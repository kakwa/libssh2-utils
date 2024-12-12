cmake_minimum_required(VERSION 3.10)

# Project name
project(LibSSH2Algorithms)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Find the libssh2 library
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH2 REQUIRED libssh2)

# Include the directories for libssh2
include_directories(${LIBSSH2_INCLUDE_DIRS})

# Add the executable
add_executable(libssh2-list-ciphers list-ciphers.c)

# Link the libssh2 library
target_link_libraries(libssh2-list-ciphers ${LIBSSH2_LIBRARIES})

# Add compile options (optional, useful for warnings)
target_compile_options(libssh2-list-ciphers PRIVATE ${LIBSSH2_CFLAGS_OTHER})
