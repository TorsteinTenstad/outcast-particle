vec4 blend(vec4 base, vec4 top)
{
	float a = top.a + base.a * (1 - top.a);
	if (abs(a) < 0.0001)
	{
		return vec4(0);
	}
	vec3 rgb = (top.a * top.rgb + base.a * base.rgb * (1 - top.a)) / a;
	return vec4(rgb, a);
}