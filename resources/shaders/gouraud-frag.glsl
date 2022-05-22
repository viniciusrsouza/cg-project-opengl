#version 330 core
out vec4 color;

struct Material {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

struct Light {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 position;
};

in vec3 Normal;
in vec3 frag_pos;

uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main() {
  vec3 norm = normalize(Normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  vec3 view_dir = normalize(view_pos - frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);

  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  float diff = max(dot(norm, light_dir), 0.0);
  
  vec4 ambient = light.ambient * material.ambient;
  vec4 diffuse = light.diffuse * (diff * material.diffuse);
  vec4 specular =  light.specular * (spec * material.specular);

  vec4 result = ambient + diffuse + specular;
  color = result;
  // color = vec4(0.6, 0.0, 0.5, 1.0);
}