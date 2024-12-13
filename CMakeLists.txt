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

# Define the directory containing the source files
set(CMD_DIR "${CMAKE_SOURCE_DIR}/cmd")

# Glob all .c files in the cmd directory
file(GLOB CMD_SOURCES "${CMD_DIR}/*.c")

# Initialize a list to hold the executable targets
set(EXECUTABLE_TARGETS)

# Iterate over each source file
foreach(SOURCE_FILE ${CMD_SOURCES})
  # Get the filename without the directory and extension
  get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)

  # Prepend 'libssh2-' to the executable name
  set(EXEC_NAME "libssh2-${EXEC_NAME}")

  # Add the executable
  add_executable(${EXEC_NAME} ${SOURCE_FILE})

  # Link the libssh2 library
  target_link_libraries(${EXEC_NAME} ${SSH2_LIBRARY})

  # Add the executable to the list of targets
  list(APPEND EXECUTABLE_TARGETS ${EXEC_NAME})
endforeach()

# Install the executables
install(
  TARGETS ${EXECUTABLE_TARGETS}
  RUNTIME DESTINATION ${BIN_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
