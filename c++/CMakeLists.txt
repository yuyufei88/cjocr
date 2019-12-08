cmake_minimum_required(VERSION 3.14)
project(cj_ocr)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(cj_ocr main.cpp cjocr.h cjocr.cpp)
target_link_libraries(cj_ocr ${OpenCV_LIBS})