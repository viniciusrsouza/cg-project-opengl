#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
  Camera(
      glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
      float yaw = -90.0f,
      float pitch = 0.0f,
      float move_speed = 2.5f,
      float sensitivity = 0.1f,
      float zoom = 45.0f,
      float fovy = 45.0f,
      float near = 5.0f,
      float far = 10000.0f);
  ~Camera();

  enum Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

  glm::mat4 getView();
  glm::mat4 getProjection(float aspect);

  void move(Camera::Movement direction, float delta_time);
  void rotate(float xoffset, float yoffset, bool constraint = true);

  glm::vec3 p;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  // euler Angles
  float yaw;
  float pitch;

  // view section
  float near;
  float far;
  float fovy;

  // camera options
  float move_speed;
  float sensitivity;
  float zoom;

private:
  void update();
};