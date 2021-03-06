#pragma once
#include <glad/glad.h>
#include <core/shader.h>

struct Material {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;
};

class Shape
{
public:
  Shape();
  ~Shape();

  void draw();
  void bind();
  void withShader(Shader *shader);

  Material material;
  glm::vec3 position;
  glm::vec3 scale;
  float *vertices;
  unsigned int *indices;
  unsigned int VBO, VAO, EBO;
  unsigned int vertex_size, index_size;
};

namespace Primitives
{
  Shape *getCube();
  Shape *getFloor();
};