#shader vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTexCoords;


out VS_OUT{
    vec3 FragPos;
    vec3 TexCoords;
} vs_out;

uniform mat4 u_VP;
uniform mat4 u_Model;



void main()
{
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    gl_Position = u_VP * u_Model * vec4(aPos, 1.0);
}

#shader fragment
#version 430 core
out vec4 FragColor;




in VS_OUT{
    vec3 FragPos;
    vec3 TexCoords;
} fs_in;

uniform layout(rgba32f) image2D image;

void main()
{
    ivec2 coords = ivec2(fs_in.FragPos.xz);
    vec3 color = imageLoad(image, coords).rgb;
    FragColor = vec4(1, 1, 1, 1.0);
}