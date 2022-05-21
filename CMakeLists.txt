cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_custom_target(run
  COMMAND cgproject
  DEPENDS cgproject
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
project(cgproject VERSION 1.0)


add_subdirectory(vendors)
add_subdirectory(app)

add_executable(cgproject ${SRC})

target_include_directories(cgproject PRIVATE ${INC})
target_link_libraries(
  cgproject 
  glfw
  glm
)