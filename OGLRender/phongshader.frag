#version 130


in vec3 P;
in vec3 N;

out vec3 color_final;

uniform vec3 color = vec3(1.0,0.0,0.0);
uniform vec3 bcolor = vec3(1.0,0.0,1.0);

const vec3 lp1 = vec3(1000,1000,1000);
const float specExp = 100;

void main()
{
	vec3 No = normalize(N);
	vec3 Lu1 = normalize(lp1-P);
	if (gl_FrontFacing)
	{
		float lambert= 0.2 + 0.8 * clamp(dot(No,Lu1),0,1);
		vec3 Re = reflect(-Lu1,No);
		vec3 Ca = normalize(-P);
		float spec = pow( clamp(dot(Re,Ca),0,1), specExp);
		color_final = color*lambert + vec3(1.0,1.0,1.0)*spec;
	}
	else
	{
		float lambert= 0.2 +0.8*clamp(-dot(No,Lu1),0,1);
		color_final = bcolor*lambert;
	}
}

