#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;



uniform mat4 u_VP;
uniform mat4 u_Model;

out VS_OUT{
    vec3 vertex;
    vec3 normal;
} vs_out;


void main()
{
    vs_out.vertex = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.normal = vec3(u_Model * vec4(aNormal, 1.0));
    gl_Position = u_VP * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 vertex;
    vec3 normal;
} fs_in;

void main()
{
    vec3 light = normalize(vec3(500, 1000, 100) - fs_in.vertex);
    vec3 normal = normalize(fs_in.normal);
    float diff = max(dot(normal, light), 0.5);
    vec3 color = vec3(1, 0.2, 0.4);

    if (max(dot(vec3(0, 1, 0), normal), 0) > 0.75)
        color = vec3(0, 1, 0.2);
    color = fs_in.normal;
    FragColor = vec4(color*diff, 1.0);
}