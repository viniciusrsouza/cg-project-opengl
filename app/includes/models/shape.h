#pragma once
#include <glad/glad.h>
#include <core/shader.h>

class Shape
{
public:
  Shape();
  ~Shape();

  void draw();
  void bind();

  glm::vec3 position;
  glm::vec3 scale;
  float *vertices;
  unsigned int *indices;
  unsigned int VBO, VAO, EBO;
  unsigned int vertex_size, index_size;
};

namespace Primitives
{
  Shape getCube();
  Shape getFloor();
};