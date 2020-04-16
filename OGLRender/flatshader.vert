#version 130

in vec3 vertex_in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 P;

void main()
{
	vec4 P4 = viewMatrix * vec4(vertex_in, 1.0);
	P = P4.xyz;
	gl_Position = projectionMatrix * P4;
}
