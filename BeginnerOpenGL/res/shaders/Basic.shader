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
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 u_VP;
uniform mat4 u_Model;

uniform vec3 lightPos;
uniform vec3 viewPos;



void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos = TBN * viewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;

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
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

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
    // ambient
    vec3 ambient = 0.003f * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 5.0);

    vec3 specular = texture(texture_specular1, fs_in.TexCoords).rgb * spec;

    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 10.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient += ambient* attenuation*light.color;
    diffuse += diffuse* attenuation*light.color;
    specular += specular* attenuation*light.color;

    vec3 result = pow(ambient + diffuse + 0.6f*specular, vec3(1.0 / 2.2));
    FragColor = vec4(result, 1.0);
}