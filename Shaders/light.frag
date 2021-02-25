#version 330 core

// Inputs
in vec3 f_World;
in vec3 f_Normal;

// Uniforms
uniform vec3 u_DirectionalLight;
uniform vec3 u_EyePosition;

struct Point_Light
{
	vec3 position;
	float intensity;
};

uniform Point_Light u_PointLight;

// Light data
const vec3 AMBIENT = vec3(0.4, 1.0, 0.2);
const float AMBIENT_STRENGTH = 0.05;
const float SPEC_EXPONENT = 32.0;
const float SPEC_INTENSITY = 0.8;


vec3 Light_Func(vec3 light_dir, float intensity)
{
	// Diffuse lighting
	float diffuse = dot(f_Normal, -light_dir);
	diffuse = clamp(diffuse, 0.0, 1.0);
	

	// Specular lighting (using Blinn-Phong baby)
	vec3 eye_dir = normalize(u_EyePosition - f_World);
	vec3 middle = normalize(eye_dir + -u_DirectionalLight);
	float specular = dot(f_Normal, middle);
	specular = max(specular, 0.0);
	specular = pow(specular, SPEC_EXPONENT) * SPEC_INTENSITY;

	return vec3(1.0) * (diffuse + specular) * intensity;
}

// Output
out vec4 o_Color;

void main()
{
	vec3 light_clr = vec3(0.0);

	// Add directional light
	light_clr += Light_Func(u_DirectionalLight, 0.8);

	// Add point light
	vec3 point_light_dir = normalize(f_World - u_PointLight.position);
	float dist = 1.0f / length(f_World - u_PointLight.position);
	light_clr += Light_Func(point_light_dir, u_PointLight.intensity) * dist;

	// Ambient ligthing
	vec3 ambient = AMBIENT * AMBIENT_STRENGTH;

	o_Color = vec4(ambient + light_clr, 1.0);
}