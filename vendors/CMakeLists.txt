add_subdirectory(glfw)
add_subdirectory(glm)

set(SRC
  ${SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c
  PARENT_SCOPE
)

set(INC
  ${INC}
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
  PARENT_SCOPE
)