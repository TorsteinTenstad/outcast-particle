vec4 gem(vec2 uv, float AA){
	vec2 gem_c = abs(rot(PI/4)*uv);
	vec2 gem_color_c = (rot(PI/8)*(uv+vec2(0, 0.5)));
	//vec3 gem_color = gem_color_c.x*vec3(0.8, 0.8, 0.1) + gem_color_c.y*vec3(0.9, 0.9, 0.9);
	vec3 gem_color = gem_color_c.x*vec3(0, 1, 1) + gem_color_c.y*vec3(0, 0, 1);
	float gem_alpha = 1-smoothstep(0, AA, max(gem_c.x, gem_c.y)-SQRT2*0.25);
	vec4 gem_contribution = vec4(1-gem_color, gem_alpha);
	return gem_contribution;
}