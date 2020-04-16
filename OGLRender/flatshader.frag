#version 130

in vec3 P;

out vec3 color_final;

uniform vec3 color = vec3(1.0,0.0,0.0);
uniform vec3 bcolor = vec3(0.0,1.0,1.0);

const vec3 lp1 = vec3(1000,1000,1000);


void main()
{
	vec3 N = normalize(cross(dFdx(P),dFdy(P)));
	vec3 Lu1 = normalize(lp1-P);
        float lambert= 0.2 + 0.8 * clamp(dot(N,Lu1),0,1);

	if (gl_FrontFacing)
		color_final = color*lambert;
	else
		color_final = bcolor*lambert;
}

