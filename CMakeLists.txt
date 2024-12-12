cmake_minimum_required(VERSION 3.10)

# Project name
project(LibSSH2-Utils)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Find the libssh2 library
find_package(SSH2 REQUIRED)

# Include the directories for libssh2
include_directories(${SSH2_INCLUDE_DIRS})

# Add the executable
add_executable(libssh2-list-ciphers list-ciphers.c)

# Link the libssh2 library
target_link_libraries(libssh2-list-ciphers ${SSH2_LIBRARY})

# Add compile options (optional, useful for warnings)
target_compile_options(libssh2-list-ciphers PRIVATE ${SSH2_CFLAGS_OTHER})
