cmake_minimum_required(VERSION 3.10)

# Project name
project(LibSSH2-Utils)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Specify the C standard
set(CMAKE_C_STANDARD 11)

# Find the libssh2 library
find_package(SSH2 REQUIRED)

if(NOT SSH2_FOUND)
    message(FATAL_ERROR "Could not find SSH2, please install libssh2-1-dev or equivalent package")
endif()

# Include the directories for libssh2
include_directories(${SSH2_INCLUDE_DIRS})

# Add the executable
add_executable(libssh2-list-ciphers list-ciphers.c)

# Link the libssh2 library
target_link_libraries(libssh2-list-ciphers ${SSH2_LIBRARY})

# Add the executable
add_executable(libssh2-test-remote-cipher-exchange test-remote-cipher-exchange.c)
# Link the libssh2 library
target_link_libraries(libssh2-test-remote-cipher-exchange ${SSH2_LIBRARY})


# install binaries and library
install(
  TARGETS libssh2-list-ciphers libssh2-test-remote-cipher-exchange
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
