# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\therion_preview_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\therion_preview_autogen.dir\\ParseCache.txt"
  "therion_preview_autogen"
  )
endif()
