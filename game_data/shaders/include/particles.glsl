float particle(vec2 uv, float charge_sign)
{
	float AA = 0.01;
	float LINES_WIDTH = 0.15;
	float SIGN_SIZE = 0.2;

	float r = length(uv);
	vec2 uv_4 = abs(uv);
	vec2 uv_8 = vec2(max(uv_4.x, uv_4.y), min(uv_4.x, uv_4.y));
	vec2 uv_sign = charge_sign < 0 ? uv_4 : uv_8;
	float pluss_base = smoothstep(0, AA, LINES_WIDTH / 2.f - uv_sign.y) * smoothstep(0, AA, SIGN_SIZE - uv_sign.x);
	vec2 rounding_vector = uv_sign - vec2(SIGN_SIZE, 0);
	float pluss_tip = smoothstep(0, AA, LINES_WIDTH / 2.f - length(rounding_vector));
	float pluss = max(pluss_base, pluss_tip);
	float ring_inner_mask = smoothstep(0, AA, r - 0.5 + LINES_WIDTH);
	float ring_outer_mask = smoothstep(0, AA, 0.5 - r);
	float ring = ring_inner_mask * ring_outer_mask;
	return ring_outer_mask - pluss;
}