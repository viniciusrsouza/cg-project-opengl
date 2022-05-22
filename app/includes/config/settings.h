#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

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
      float scale;
      glm::vec3 position;
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
  static Node encode(const glm::vec3 &v)
  {
    return Node();
  }

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
struct convert<Settings::Object> {
  static Node encode(const Settings::Object &obj)
  {
    return Node();
  }

  static bool decode(const Node &node, Settings::Object &obj)
  {
    if (!node.IsMap())
      return false;
    obj.file = node["file"].as<std::string>();
    obj.scale = node["scale"].as<float>();
    obj.position = node["position"].as<glm::vec3>();
    return true;
  }
};

template<>
struct convert<Settings::Config> {
  static Node encode(const Settings::Config &conf)
  {
    return Node();
  }

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