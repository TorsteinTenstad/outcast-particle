#define PI 3.1415926535897932384626433832795

float radial_falloff(float r, float a, float b)
{
	float f = 1 / (max((r - a), 0) / (b - a) + 1);
	return smoothstep(0.5, 1, f);
}

mat2 rot(float theta) {
	float s = sin(theta);
	float c = cos(theta);
	mat2 m = mat2(c, -s, s, c) ;
    return m;
}

mat2 inverse(mat2 m)
{
	return mat2(m[1][1], -m[0][1], -m[1][0], m[0][0]) / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
}

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}