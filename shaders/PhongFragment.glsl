#version 330 core
/// @brief our output fragment colour
//layout (location =0) out vec4 fragColour;
in vec3 normal;
in vec3 fragPos;

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
uniform vec3 viewerPos;
//uniform vec3 lightColour;

void main()
{
    vec3 lightDir = normalize(light.position.xyz - fragPos.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewerPos - fragPos);
    float ambientStr = 0.2;
    float diffuseStr = dot(normal, lightDir);
    float specularStr = 0.3;
    // ambient component
    vec4 ambient = vec4(ambientStr*light.diffuse)*objectColour;
    //diffuse component
    vec4 diffuse = diffuseStr*light.diffuse*objectColour;
    // specular component
    float spec = pow(max(dot(viewDir, reflectDir),0.0),32);
    vec4 specular = specularStr*spec*light.specular*objectColour;



    colour = diffuse + ambient + specular;
}

