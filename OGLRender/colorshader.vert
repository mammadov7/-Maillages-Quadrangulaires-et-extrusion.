#version 130

in vec3 vertex_in;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(vertex_in, 1.0);
}
