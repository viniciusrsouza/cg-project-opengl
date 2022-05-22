#include <iostream>
#include <vector>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <yaml-cpp/yaml.h>

#include <core/shader.h>
#include <core/camera.h>
#include <models/shape.h>
#include <models/parser.h>
#include <config/settings.h>

int init(GLFWwindow **window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// camera settings
float delta_time = 0.0f;
float last_frame = 0.0f;

float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
float first_mouse = true;

Camera *camera;

// settings
Settings::Config cfg;

// models
Shape *light;
Shape *floor_shape;
Shape **models;
unsigned int model_count;

// loaders
int load();

int main(void)
{
  GLFWwindow *window;
  int err = init(&window);
  if (err != 0)
  {
    std::cerr << "Application failed to initialize with code: " << err << std::endl;
    return err;
  }

  load();

  // load shaders and light
  Shader shader("resources/shaders/projection-vert.glsl", "resources/shaders/gouraud-frag.glsl");
  Shader light_shader("resources/shaders/light-vert.glsl", "resources/shaders/light-frag.glsl");

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* render here */
    glm::mat4 view = camera->getView();
    glm::mat4 projection = camera->getProjection((float)SCR_WIDTH / (float)SCR_HEIGHT);

    light_shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cfg.light.position);
    model = glm::scale(model, glm::vec3(2.2f));

    light_shader.setMat4("model", model);
    light_shader.setMat4("projection", projection);
    light_shader.setMat4("view", view);
    light_shader.setVec4("light_color", cfg.light.diffuse);
    light->draw();

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("view_pos", camera->p);
    shader.setVec4("light.ambient", cfg.light.ambient);
    shader.setVec4("light.diffuse", cfg.light.diffuse);
    shader.setVec4("light.specular", cfg.light.specular);
    shader.setVec3("light.position", cfg.light.position);

    if (cfg.show_floor)
    {
      floor_shape->withShader(&shader);
      floor_shape->draw();
    }

    for (int i = 0; i < model_count; i++)
    {
      auto obj = models[i];
      obj->withShader(&shader);
      obj->draw();
    }

    glfwSwapBuffers(window);
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
  }

  return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
  SCR_WIDTH = width;
  SCR_HEIGHT = height;
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  if (first_mouse)
  {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    camera->rotate(xoffset, yoffset);
  }
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
    std::cout << "Escape key pressed, terminating..." << std::endl;
  }

  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
  {
    load();
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  else
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  // camera movements
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera->move(Camera::FORWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera->move(Camera::BACKWARD, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera->move(Camera::LEFT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera->move(Camera::RIGHT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    camera->move(Camera::UP, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    camera->move(Camera::DOWN, delta_time);
  }
}

int loadConfig()
{
  YAML::Node config = YAML::LoadFile("config.yaml");
  if (!config["settings"])
  {
    std::cout << "Could not find settings in config.yaml" << std::endl;
    return 1;
  }
  cfg = config["settings"].as<Settings::Config>();
  return 0;
}

int loadCamera()
{
  camera = new Camera(
      cfg.camera.position,
      glm::vec3(0.0f, 1.0f, 0.0f),
      cfg.camera.yaw,
      cfg.camera.pitch,
      cfg.camera.speed,
      0.1f,
      45.0f,
      cfg.camera.fovy,
      cfg.camera.near,
      cfg.camera.far);
  return 0;
}

int loadModels()
{
  delete light;
  delete floor_shape;
  delete[] models;

  // light model
  light = Primitives::getCube();
  light->position = cfg.light.position;
  light->bind();

  // floor_shape model
  floor_shape = Primitives::getFloor();
  floor_shape->scale = glm::vec3(15000.0f, 1.0f, 15000.0f);
  floor_shape->position = glm::vec3(0.0f, 0.0f, 0.0f);
  floor_shape->material = {
      glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
      50.0f,
  };
  floor_shape->bind();

  // parameterized models
  model_count = cfg.objects.size();
  models = new Shape *[model_count];
  for (int i = 0; i < model_count; i++)
  {
    auto obj = Shapes::from_file(cfg.objects[i].file);
    obj->position = cfg.objects[i].position;
    obj->material = cfg.objects[i].material;
    obj->scale = glm::vec3(cfg.objects[i].scale);
    obj->bind();
    models[i] = obj;
  }

  return 0;
}

int load()
{
  int cgf_err = loadConfig();
  int cam_err = loadCamera();
  int mod_err = loadModels();
  if (cgf_err || cam_err || mod_err)
    return 1;
  return 0;
}

int init(GLFWwindow **window)
{
  if (!glfwInit())
  {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  std::cout << "Apple system, runnning in compat mode." << std::endl;
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG Project", NULL, NULL);

  if (*window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -2;
  }

  glfwMakeContextCurrent(*window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -3;
  }

  unsigned int major = glfwGetWindowAttrib(*window, GLFW_CONTEXT_VERSION_MAJOR);
  unsigned int minor = glfwGetWindowAttrib(*window, GLFW_CONTEXT_VERSION_MINOR);
  std::cout << "Shader version: " << major << "." << minor << std::endl;

  std::cout << "Initialized OpenGL context" << std::endl;
  std::cout << glGetString(GL_VERSION) << std::endl;

  glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);
  glfwSetCursorPosCallback(*window, mouseCallback);

  glEnable(GL_DEPTH_TEST);

  return 0;
}