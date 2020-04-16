#version 130


in vec3 vertex_in;
in vec3 normal_in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

out vec3 P;
out vec3 N;

void main()
{
	vec4 N4 = normalMatrix * vec4(normal_in, 1.0);
	N = N4.xyz;

	vec4 P4 = viewMatrix * vec4(vertex_in, 1.0);
	P = P4.xyz;

	gl_Position = projectionMatrix * P4;
}
