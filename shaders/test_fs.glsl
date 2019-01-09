#version 130

in vec3 position_eye, normal_eye;
in vec2 st;
uniform sampler2D rgb_sampler;
uniform sampler2D spec_sampler;
uniform mat4 view;

// fixed point light properties
vec3 light_position_world  = vec3 (100.0, 100.0, 100.0);
vec3 Ls = vec3 (1.0, 1.0, 0.7); // white specular colour
vec3 Ld = vec3 (0.9, 0.9, 0.9); // dull white diffuse light colour
vec3 La = vec3 (0.0, 0.0, 0.0); // grey ambient colour
  
// surface reflectance
vec3 Ks = vec3 (0.7, 0.7, 0.7); // fully reflect specular light
vec3 Kd = vec3 (1.0, 1.0, 1.0); // diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 10.0; // specular 'power'

out vec4 fragment_colour; // final colour of surface
void main () {
	// raise light position to eye space
	vec3 light_pos_eye = vec3(view * vec4 (light_position_world, 1.0));
	vec3 dist_light_eye = normalize(light_pos_eye - position_eye);
	vec3 dir_light_eye = normalize (dist_light_eye);

	// (1) ambient intensity
	vec3 Ia = La * Ka;

	// (2) diffuse intensity
	vec3 Id = Ld * Kd * max(dot(dir_light_eye, normal_eye), 0.0f);

	// (3) specular intensity 
	vec3 surf_view_eye = normalize (-position_eye);
    	vec3 Is = Ls * Ks * pow( max( dot(reflect(-dist_light_eye, normal_eye), surf_view_eye), 0.0f), specular_exponent);
	
	// final colour
	vec4 texel = texture (rgb_sampler, st);
	vec4 spectexel = texture (spec_sampler, st);
	fragment_colour = vec4 (Is*(spectexel.xyz) + (Id + Ia)*texel.xyz, 1.0);
	//fragment_colour = spectexel;
}
