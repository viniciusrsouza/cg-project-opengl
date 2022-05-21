set(SRC
  ${SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/file.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/shader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/models/shape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/models/parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  PARENT_SCOPE
)

set(INC
  ${INC}
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
  PARENT_SCOPE
)