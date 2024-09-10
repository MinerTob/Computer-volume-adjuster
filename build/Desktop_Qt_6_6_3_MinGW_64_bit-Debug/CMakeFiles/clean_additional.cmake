# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\computer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\computer_autogen.dir\\ParseCache.txt"
  "computer_autogen"
  )
endif()
