cmake_minimum_required(VERSION 3.25)

project(flying-object-recognition-system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_AUTOMOC True)

add_compile_options(-Wall -Wextra)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
  app
  src/main.cpp
  src/main_window.cpp
)

target_link_libraries(app PRIVATE Qt6::Widgets)
target_link_libraries(app PRIVATE ${OpenCV_LIBS})
