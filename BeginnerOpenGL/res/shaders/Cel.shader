#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} vs_out;

uniform mat4 u_VP;
uniform mat4 u_Model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = vec3(u_Model * vec4(aNormal, 1.0));

    gl_Position = u_VP * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct PointLight 
{
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform PointLight light;

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // get diffuse color


    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;


    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, fs_in.Normal), 0.0);
    vec3 diffuse = diff * color;


    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 4.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse += diffuse* attenuation*light.color;

    vec3 result = pow(diffuse, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0);
}