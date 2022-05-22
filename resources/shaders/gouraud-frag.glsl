#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 frag_pos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
  vec3 norm = normalize(Normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 16);
  vec3 specular = 0.5 * light_color * spec;

  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = diff * light_color;

  float ambient_strength = 0.1;
  vec3 ambient = ambient_strength * light_color;

  vec3 result = (ambient + diffuse + specular) * object_color;
  color = vec4(result, 1.0);
  // color = vec4(0.6, 0.0, 0.5, 1.0);
}