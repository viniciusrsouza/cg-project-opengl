#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
  // program ID
  unsigned int ID;

  // loads shader from file paths
  Shader(const char *vertexPath, const char *fragmentPath);
  ~Shader();

  // activate the shader
  void use();

  // uniforms
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 &mat) const;
  void setVec3(const std::string &name, glm::vec3 &vec) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, glm::vec4 &vec) const;
  void setVec4(const std::string &name, float x, float y, float z, float w) const;

private:
  std::string vertex_filename;
  std::string fragment_filename;

  std::string getFileName(unsigned int type);
  unsigned int compileShader(unsigned int type, const std::string &source, const std::string &filename);
  unsigned int createShader(
      const std::string &vertex_source,
      const std::string &fragment_source,
      const std::string &vertex_filename,
      const std::string &fragment_filename);
};