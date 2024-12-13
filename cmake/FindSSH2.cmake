# Look for the SSH2 library and headers

find_path(SSH2_INCLUDE_DIR libssh2.h)
find_library(SSH2_LIBRARY NAMES ssh2)

# Check if the library and headers were found

if(SSH2_INCLUDE_DIR AND SSH2_LIBRARY)
  set(SSH2_FOUND TRUE)
endif()

# Report the results

if(SSH2_FOUND)
  message(STATUS "Found SSH2: ${SSH2_INCLUDE_DIR}, ${SSH2_LIBRARY}")
else()
  message(STATUS "Could not find SSH2")
endif()

# Define the SSH2 target

if(SSH2_FOUND)
  add_library(SSH2::SSH2 INTERFACE IMPORTED)
  target_include_directories(SSH2::SSH2 INTERFACE ${SSH2_INCLUDE_DIR})
  target_link_libraries(SSH2::SSH2 INTERFACE ${SSH2_LIBRARY})
endif()
