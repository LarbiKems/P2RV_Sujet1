CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (P2RV_Sujet1)

set (EXECUTABLE_OUTPUT_PATH bin)

set (
  SOURCE_FILES
  src/main.cpp src/faceDetection.cpp src/faceDetection.hpp
)

# OpenCV
find_package ( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


# adding the source files to the executable
add_executable( P2RV_Sujet1 ${SOURCE_FILES})
target_link_libraries( P2RV_Sujet1 ${OpenCV_LIBS})

