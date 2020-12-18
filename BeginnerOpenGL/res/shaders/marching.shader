#shader vertex
#version 430

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat4 u_VP;
uniform mat4 u_Model;

out VS_OUT{
    vec3 vertex;
    vec3 normal;
} vs_out;


void main()
{
    vs_out.vertex = vec3(u_Model * vec4(vertex, 1.0));
    vs_out.normal = vec3(u_Model * vec4(normal, 1.0));

	gl_Position = u_VP * u_Model * vec4(vertex, 1.0);
}


#shader fragment
#version 430

uniform sampler2D diffuse_texture;
uniform sampler2D grass_texture;
uniform sampler2D snow_texture;
uniform sampler2D sand_texture;

in VS_OUT{
	vec3 vertex;
	vec3 normal;
} fs_in;

out vec4 frag_color;


void main()
{
	vec3 rock = vec3(0.4, 0.3, 0.2);
	vec3 grass = vec3(0.3, 0.7, 0.2);
	vec3 sand = vec3(0.9, 0.8, 0.6);
	vec3 snow = vec3(0.9, 0.9, 0.9);
	float texscale = 0.0001;
	vec3 light = normalize(vec3(500, 1000, 100) - fs_in.vertex);
	vec3 normal = normalize(fs_in.normal);
	float diff = max(dot(normal, light), 0.5);
	vec3 tex;
	if (max(dot(vec3(0, 1, 0), normal), 0) < 0.75) {
		if (fs_in.vertex.y < 7.5) {
			tex = texture(diffuse_texture, ivec2(int(fs_in.vertex.x), int(fs_in.vertex.z)) * texscale).rgb;
			tex = rock;
		}
	}
	else {
		tex = texture(grass_texture, fs_in.vertex.xy*texscale).rgb;
		tex = grass;
	}

	if (fs_in.vertex.y > 7.5) {
		tex = texture(diffuse_texture, fs_in.vertex.xz* texscale).rgb;
		tex = rock;

		if (max(dot(vec3(0, 1, 0), normal), 0) > 0.75) {
			tex = texture(snow_texture, fs_in.vertex.xz* texscale).rgb;
			tex = snow;
		}
	}
	if (fs_in.vertex.y < 0.3 && max(dot(vec3(0, 1, 0), normal), 0)> 0.65) {
		tex = texture(sand_texture, vec2(fs_in.vertex.x, fs_in.vertex.z)* texscale).rgb;
		tex = sand;
	}
	vec3 color = tex;
	frag_color = vec4(color * diff, 1);
}