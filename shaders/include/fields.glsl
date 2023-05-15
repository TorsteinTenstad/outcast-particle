

float get_linearized_log2_field_strength01(float field_strength, float min_strength, float max_field_strength){
	return (log2(field_strength)-log2(min_strength))/(log2(max_field_strength)-log2(min_strength));
}

float get_field_thickness_factor(float field_strength01){
	float thickness_variation = 0.2;
	float thickness_factor = 1-thickness_variation + 2*thickness_variation*field_strength01;
	return thickness_factor;
}

vec3 get_field_rgb(vec2 uv, float field_strength01){
	return mix(LIGHT_PURPLE, PURPLE, 2*mix(field_strength01, uv.y, 0.3));
}