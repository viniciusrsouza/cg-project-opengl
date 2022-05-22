#version 330 core
out vec4 color;

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

in vec3 Normal;
in vec3 frag_pos;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform Material material;

void main() {
  vec3 norm = normalize(Normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  vec3 specular =  light_color * (spec * vec3(material.specular));

  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light_color * (diff * vec3(material.diffuse));

  vec3 ambient = light_color * vec3(material.ambient);

  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0);
  // color = vec4(0.6, 0.0, 0.5, 1.0);
}