#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <models/shape.h>

namespace Settings
{
  class Object
  {
    public:
      Object() {}
      ~Object() {}

      friend std::ostream &operator<<(std::ostream &os, Object const &obj)
      {
        os << "file: " << obj.file << "; scale: " << obj.scale;
        return os;
      }

      std::string file;
      glm::vec3 position;
      Material material;
      float scale;
  };
  class Config
  {
    public:
      Config() {}
      ~Config() {}

      friend std::ostream &operator<<(std::ostream &os, Config const &cfg)
      {
        os << "Config: " << std::endl;
        os << "  objects: " << std::endl;
        for (auto obj : cfg.objects)
        {
          os << "    - " << obj << std::endl;
        }
        return os;
      }

      std::vector<Object> objects;
  };
}

namespace YAML
{
template<>
struct convert<glm::vec3> {
  static bool decode(const Node &node, glm::vec3 &v)
  {
    if (!node.IsSequence())
      return false;
    if (node.size() != 3)
      return false;
    v.x = node[0].as<float>();
    v.y = node[1].as<float>();
    v.z = node[2].as<float>();
    return true;
  }
};

template<>
struct convert<glm::vec4> {
  static bool decode(const Node &node, glm::vec4 &v)
  {
    if (!node.IsSequence())
      return false;
    if (node.size() != 4)
      return false;
    v.x = node[0].as<float>();
    v.y = node[1].as<float>();
    v.z = node[2].as<float>();
    v.w = node[3].as<float>();
    return true;
  }
};

template<>
struct convert<Material> {
  static bool decode(const Node &node, Material &mat)
  {
    if (!node.IsMap())
      return false;
    mat.ambient = node["ambient"].as<glm::vec4>();
    mat.diffuse = node["diffuse"].as<glm::vec4>();
    mat.specular = node["specular"].as<glm::vec4>();
    mat.shininess = node["shininess"].as<float>();
    return true;
  }
};

template<>
struct convert<Settings::Object> {
  static bool decode(const Node &node, Settings::Object &obj)
  {
    if (!node.IsMap())
      return false;
    obj.file = node["file"].as<std::string>();
    obj.scale = node["scale"].as<float>();
    obj.position = node["position"].as<glm::vec3>();
    obj.material = node["material"].as<Material>();
    return true;
  }
};

template<>
struct convert<Settings::Config> {
  static bool decode(const Node &node, Settings::Config &conf)
  {
    if (node["objects"])
    {
      int i = 0;
      conf.objects = std::vector<Settings::Object>();
      auto it = node["objects"].begin();
      while (it != node["objects"].end())
      {
        std::cout << "parsing: " << it->first.as<std::string>() << std::endl;
        std::cout << "file: " << it->second["file"].as<std::string>() << std::endl;
        auto obj = it->second.as<Settings::Object>();
        conf.objects.push_back(obj);
        it++;
      }
    }

    return true;
  }
};
}