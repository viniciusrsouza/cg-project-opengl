#include <models/shape.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include <iostream>

Shape::Shape() 
{
  material = {
    glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), // ambient
    glm::vec4(0.7f, 0.5f, 0.8f, 1.0f), // diffuse
    glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), // specular
    100.0f // shininess
  };
}
Shape::~Shape()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  delete[] vertices;
  delete[] indices;
}

void Shape::bind()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

  // bind index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);

  // set position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // set normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind
  glBindVertexArray(0);
}

void Shape::withShader(Shader *shader)
{
  shader->setVec4("material.ambient", material.ambient);
  shader->setVec4("material.diffuse", material.diffuse);
  shader->setVec4("material.specular", material.specular);
  shader->setFloat("material.shininess", material.shininess);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, scale);
  shader->setMat4("model", model);
}

void Shape::draw()
{
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, index_size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

Shape* Primitives::getCube()
{
  Shape *cube = new Shape();
  float vertices[] = {
      // positions          // normals
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f};

  unsigned int indices[] = {
      0, 1, 3,
      0, 3, 2,
      0, 1, 4,
      0, 4, 5,
      4, 5, 7,
      4, 7, 6,
      6, 7, 3,
      6, 3, 2,
      0, 4, 6,
      0, 6, 2,
      1, 5, 7,
      1, 7, 5
      };

  cube->vertex_size = sizeof(vertices);
  cube->vertices = (float*) malloc(sizeof(vertices));
  memcpy(cube->vertices, vertices, sizeof(vertices));
  cube->index_size = sizeof(indices);
  cube->indices = (unsigned int*) malloc(sizeof(indices));
  memcpy(cube->indices, indices, sizeof(indices));  

  return cube;
}

Shape* Primitives::getFloor()
{
  Shape *floor = new Shape();
  float vertices[] = {
      // positions          // normals
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
  };

  unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3
  };

  floor->vertex_size = sizeof(vertices);
  floor->vertices = (float*) malloc(sizeof(vertices));
  memcpy(floor->vertices, vertices, sizeof(vertices));
  floor->index_size = sizeof(indices);
  floor->indices = (unsigned int*) malloc(sizeof(indices));
  memcpy(floor->indices, indices, sizeof(indices));  

  return floor;
}