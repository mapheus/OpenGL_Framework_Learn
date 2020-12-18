#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main()
{
    vec3 _position = aPos + aNormal * 0.03f;
    gl_Position = u_VP * u_Model * vec4(_position, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;


void main()
{

    FragColor = vec4(vec3(0.03, 0.03, 0.03), 1.0);
}