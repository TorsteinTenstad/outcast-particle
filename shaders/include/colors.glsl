#define RED vec3(0.88671875, 0.109375, 0.1015625)
#define LIGHT_RED vec3(0.98046875, 0.6015625, 0.59765625)
#define GREEN vec3(0.19921875, 0.625, 0.171875)
#define LIGHT_GREEN vec3(0.6953125, 0.87109375, 0.5390625)
#define BLUE vec3(0.1171875, 0.46875, 0.703125)
#define LIGHT_BLUE vec3(0.6484375, 0.8046875, 0.88671875)

vec3 get_flat_particle_color(float charge_sign)
{
	return charge_sign < 0
		? mix(GREEN, LIGHT_GREEN, 0.2)
		: mix(RED, LIGHT_RED, 0.4);
}