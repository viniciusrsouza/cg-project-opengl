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
#include <models/shape.h>
#include <models/parser.h>
#include <config/settings.h>

int init(GLFWwindow **window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void)
{
  GLFWwindow *window;
  int err = init(&window);
  if (err != 0)
  {
    std::cerr << "Application failed to initialize with code: " << err << std::endl;
    return err;
  }

  YAML::Node config = YAML::LoadFile("config.yaml");
  if (!config["settings"])
  {
    std::cout << "Could not find settings in config.yaml" << std::endl;
    return 1;
  }
  Settings::Config cfg = config["settings"].as<Settings::Config>();

  Shader shader("resources/shaders/projection-vert.glsl", "resources/shaders/gouraud-frag.glsl");
  Shader light_shader("resources/shaders/light-vert.glsl", "resources/shaders/light-frag.glsl");

  // Light
  // unsigned int lightVAO;
  // glGenVertexArrays(1, &lightVAO);
  // glBindVertexArray(lightVAO);

  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  // glEnableVertexAttribArray(0);

  glm::vec3 light_position(-1.0f, 0.5f, 0.0f);
  glm::vec3 light_color(1.0f, 1.0f, 1.0f);

  Shape light = Primitives::getCube();
  light.position = light_position;
  light.bind();

  Shape cube = Primitives::getCube();
  cube.position = glm::vec3(-1.5f, -1.2f, -2.5f);
  cube.bind();

  int obj_count = cfg.objects.size();
  Shape** objects = (Shape**) malloc(sizeof(Shape) * obj_count);
  std::cout << "count: " << obj_count << std::endl;
  for (int i = 0; i < obj_count; i++)
  {
    Shape* obj = Shapes::from_file(cfg.objects[i].file);
    obj->position = cfg.objects[i].position;
    obj->scale = glm::vec3(cfg.objects[i].scale);
    obj->bind();
    objects[i] = obj;
  }

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* render here */
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    light_shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, light_position);
    model = glm::scale(model, glm::vec3(0.2f));

    light_shader.setMat4("model", model);
    light_shader.setMat4("projection", projection);
    light_shader.setMat4("view", view);
    light_shader.setVec3("light_color", light_color);
    light.draw();

    shader.use();
    shader.setVec3("object_color", 1.0f, 0.5f, 0.31f);
    shader.setVec3("light_color", light_color);
    shader.setVec3("light_pos", light_position);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    
    model = glm::translate(glm::mat4(1.0f), cube.position);
    shader.setMat4("model", model);
    cube.draw();

    for (int i=0; i < obj_count; i++)
    {
      auto obj = objects[i];
      model = glm::mat4(1.0f);
      model = glm::translate(model, obj->position);
      model = glm::scale(model, obj->scale);
      shader.setMat4("model", model);
      obj->draw();
    }

    // glBindVertexArray(VAO);
    // for (unsigned int i = 0; i < cube_count; i++)
    // {
    //   glm::mat4 model = glm::mat4(1.0f);
    //   model = glm::translate(model, cubePositions[i]);
    //   float angle = 20.0f * i;
    //   model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    //   shader.setMat4("model", model);

    //   glDrawArrays(GL_TRIANGLES, 0, 36);
    // }

    glfwSwapBuffers(window);
  }

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

  glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
  // glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
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
  } else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}