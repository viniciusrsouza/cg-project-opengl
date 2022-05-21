#include <models/parser.h>
#include <iostream>
#include <fstream>

static auto parse_byu(std::string path)
{
  struct data {
    float* vertices;
    unsigned int* indices;
    int vert_count;
    int triangle_count;
  };

  std::ifstream file(path);
  if (!file.is_open())
  {
    std::cout << "Could not open file: " << path << std::endl;
    return data{nullptr, nullptr, 0, 0};
  }
  std::cout << "Parsing byu file: " << path << std::endl;

  int vert_count = 0;
  int triangle_count = 0;
  file >> vert_count;
  file >> triangle_count;

  float* vertices = new float[vert_count * 6];
  unsigned int* indices = new unsigned int[triangle_count * 3];

  for (int i=0; i<vert_count; i++)
  {
    file >> vertices[i*6];
    file >> vertices[i*6+1];
    file >> vertices[i*6+2];
    vertices[i*6+3] = 0.0f;
    vertices[i*6+4] = 0.0f;
    vertices[i*6+5] = 0.0f;
  }

  for (int i=0; i<triangle_count; i++)
  {
    file >> indices[i*3];
    file >> indices[i*3+1];
    file >> indices[i*3+2];
  }

  file.close();

  return data{vertices, indices, vert_count, triangle_count};
}

void fill_normals(float* vertices, unsigned int* indices, int vert_count, int triangle_count)
{
  for (int i=0; i<triangle_count; i++)
  {
    int i1 = indices[i*3];
    int i2 = indices[i*3+1];
    int i3 = indices[i*3+2];

    glm::vec3 v1 = glm::vec3(vertices[i1*6], vertices[i1*6+1], vertices[i1*6+2]);
    glm::vec3 v2 = glm::vec3(vertices[i2*6], vertices[i2*6+1], vertices[i2*6+2]);
    glm::vec3 v3 = glm::vec3(vertices[i3*6], vertices[i3*6+1], vertices[i3*6+2]);

    glm::vec3 normal = glm::cross(v2-v1, v3-v1);

    vertices[i1*6+3] = normal.x;
    vertices[i1*6+4] = normal.y;
    vertices[i1*6+5] = normal.z;

    vertices[i2*6+3] = normal.x;
    vertices[i2*6+4] = normal.y;
    vertices[i2*6+5] = normal.z;

    vertices[i3*6+3] = normal.x;
    vertices[i3*6+4] = normal.y;
    vertices[i3*6+5] = normal.z;
  }
}

Shape* Shapes::from_file(std::string path)
{
  auto data = parse_byu(path);

  fill_normals(data.vertices, data.indices, data.vert_count, data.triangle_count);

  Shape* object = new Shape();
  object->vertices = data.vertices;
  object->indices = data.indices;
  object->index_size = data.triangle_count * 3 * sizeof(unsigned int);
  object->vertex_size = data.vert_count * 6 * sizeof(float);

  return object;
}