#version 330 core
/// @brief our output fragment colour
//layout (location =0) out vec4 fragColour;
in vec3 normal;
in vec3 fragPos;
in vec3 viewDir;

out vec4 colour;

// light structure
struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation;
  float spotCosCutoff;
  float quadraticAttenuation;
  float linearAttenuation;
};

// temp colours
uniform Lights light;
uniform vec4 objectColour;
//uniform vec3 lightColour;

void main()
{
    float ambientStr = 0.2;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position.xyz - fragPos.xyz);
    float diffuseStr = max(dot(norm, lightDir), 0.0);
    float specularStr = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),32);
    vec4 specular = specularStr*spec*light.specular;
    vec4 diffuse = diffuseStr*light.diffuse;
    vec4 ambient = vec4(ambientStr*light.diffuse);

    colour = (diffuse + ambient + specular)*objectColour;
}

