cmake_minimum_required(VERSION 2.8.12)
project(python_cpp_example)

option(USE_OPENCV3 OFF)

SET(OPENCV_LIBS  opencv_core opencv_highgui opencv_imgproc)

if (USE_OPENCV3)
  SET(OPENCV_LIBS  ${OPENCV_LIBS} opencv_imgcodecs)
endif()

SET(SOURCE_DIR "src/api")
include_directories(${SOURCE_DIR})
include_directories(pybind11)
add_subdirectory(pybind11)

include_directories(src/include)

add_library(seeta_face_cropper SHARED IMPORTED)
add_library(seeta_face_detector SHARED IMPORTED)
add_library(seeta_point_detector SHARED IMPORTED)
set_target_properties(seeta_face_cropper PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libSeetaFaceCropper200.so)
set_target_properties(seeta_face_detector PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libSeetaFaceDetector200.so)
set_target_properties(seeta_point_detector PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/libSeetaPointDetector200.so)


SET ( CMAKE_CXX_FLAGS "-std=c++11")
pybind11_add_module(seetaface ${SOURCE_DIR}/seetaface.cpp)
# target_link_libraries(seetaface PRIVATE seeta_face_cropper seeta_face_detector seeta_point_detector opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs)
target_link_libraries(seetaface PRIVATE seeta_face_cropper seeta_face_detector seeta_point_detector ${OPENCV_LIBS})
