#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 frag_pos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;

void main() {
  vec3 norm = normalize(Normal);
  vec3 light_dir = normalize(light_pos - frag_pos);

  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = diff * light_color;

  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_color;

  vec3 result = (ambient + diffuse) * object_color;
  color = vec4(result, 1.0);
}