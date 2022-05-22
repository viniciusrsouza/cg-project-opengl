#include <core/camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
  glm::vec3 position,
  glm::vec3 up,
  float yaw,
  float pitch,
  float move_speed,
  float sensitivity,
  float zoom,
  float fovy,
  float near,
  float far
) :
  p(position),
  front(glm::vec3(0.0f, 1.0f, -1.0f)),
  up(up),
  right(glm::vec3(1.0f, 0.0f, 0.0f)),
  world_up(up),
  yaw(yaw),
  pitch(pitch),
  move_speed(move_speed),
  sensitivity(sensitivity),
  zoom(zoom),
  fovy(fovy),
  near(near),
  far(far)
{
  update();
}

Camera::~Camera() {}

glm::mat4 Camera::getView()
{
  return glm::lookAt(p, p + front, up);
}

glm::mat4 Camera::getProjection(float aspect)
{
  return glm::perspective(glm::radians(fovy), aspect, near, far);
}

void Camera::move(Camera::Movement direction, float delta_time)
{
  float velocity = move_speed * delta_time;
  switch (direction)
  {
    case Camera::FORWARD:
      p += front * velocity;
      break;
    case Camera::BACKWARD:
      p -= front * velocity;
      break;
    case Camera::LEFT:
      p -= right * velocity;
      break;
    case Camera::RIGHT:
      p += right * velocity;
      break;
    case UP:
      p += world_up * velocity;
      break;
    case DOWN:
      p -= world_up * velocity;
      break;
  }
}

void Camera::rotate(float xoffset, float yoffset, bool constraint)
{
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (constraint)
  {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  update();
}

void Camera::update()
{
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::normalize(glm::cross(front, world_up));
  up = glm::normalize(glm::cross(right, front));
}