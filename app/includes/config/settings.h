#pragma once

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <models/shape.h>

namespace Settings
{
  struct Light
  {
    public:
      glm::vec3 position;
      glm::vec4 ambient;
      glm::vec4 diffuse;
      glm::vec4 specular;
  };
  struct Camera
  {
  public:
    glm::vec3 position;
    float fovy;
    float near;
    float far;
    float speed;
    float yaw;
    float pitch;
  };
  struct Object
  {
  public:
    std::string file;
    glm::vec3 position;
    Material material;
    float scale;
  };
  struct Config
  {
  public:
    std::vector<Object> objects;
    Camera camera;
    Light light;
    bool show_floor;
  };
}

namespace YAML
{
  template <>
  struct convert<glm::vec3>
  {
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

  template <>
  struct convert<glm::vec4>
  {
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

  template <>
  struct convert<Settings::Light>
  {
    static bool decode(const Node &node, Settings::Light &lht)
    {
      if (!node.IsMap())
        return false;

      lht.position = node["position"].as<glm::vec3>();
      lht.ambient = node["ambient"].as<glm::vec4>();
      lht.diffuse = node["diffuse"].as<glm::vec4>();
      lht.specular = node["specular"].as<glm::vec4>();
      return true;
    }
  };

  template <>
  struct convert<Settings::Camera>
  {
    static bool decode(const Node &node, Settings::Camera &cam)
    {
      if (!node.IsMap())
        return false;

      cam.position = node["position"].as<glm::vec3>();
      cam.fovy = node["fovy"].as<float>();
      cam.near = node["near"].as<float>();
      cam.far = node["far"].as<float>();
      cam.speed = node["speed"].as<float>();
      cam.yaw = node["yaw"].as<float>();
      cam.pitch = node["pitch"].as<float>();
      return true;
    }
  };

  template <>
  struct convert<Material>
  {
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

  template <>
  struct convert<Settings::Object>
  {
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

  template <>
  struct convert<Settings::Config>
  {
    static bool decode(const Node &node, Settings::Config &conf)
    {
      std::map<std::string, Material> materials;
      if (node["materials"])
      {
        auto it = node["materials"].begin();
        while (it != node["materials"].end())
        {
          materials[it->first.as<std::string>()] = it->second.as<Material>();
          it++;
        }
      }

      if (node["objects"])
      {
        int i = 0;
        conf.objects = std::vector<Settings::Object>();
        auto it = node["objects"].begin();
        while (it != node["objects"].end())
        {
          auto obj = it->second.as<Settings::Object>();
          std::string material_name = it->second["material"].as<std::string>();
          obj.material = materials[material_name];
          conf.objects.push_back(obj);
          it++;
        }
      }

      conf.camera = node["camera"].as<Settings::Camera>();
      conf.light = node["light"].as<Settings::Light>();
      conf.show_floor = node["misc"]["show_floor"].as<bool>();
      return true;
    }
  };
}