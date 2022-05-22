#include <core/shader.h>
#include <core/file.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const char *vertex_path, const char *fragment_path)
{
  std::string vertex_source = readFile(vertex_path);
  std::string fragment_source = readFile(fragment_path);

  ID = createShader(vertex_source, fragment_source, vertex_path, fragment_path);
}

Shader::~Shader()
{
  glDeleteProgram(ID);
}

void Shader::use()
{
  glUseProgram(ID);
}

unsigned int Shader::createShader(
    const std::string &vertex_source,
    const std::string &fragment_source,
    const std::string &vertex_filename,
    const std::string &fragment_filename)
{
  unsigned int program = glCreateProgram();
  unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertex_source, vertex_filename);
  unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragment_source, fragment_filename);

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source, const std::string &filename)
{
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile shader: " << filename << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setVec3(const std::string &name, glm::vec3 &vec) const
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVec4(const std::string &name, glm::vec4 &vec) const
{
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}