#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

float radial_falloff(float r, float a, float b)
{
	float f = 1 / (max((r - a), 0) / (b - a) + 1);
	return smoothstep(0.5, 1, f);
}

mat2 rot(float theta)
{
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

float ease(float t, float undershoot, float overshoot)
{
    t = clamp(t, 0, 1);
    float a =    overshoot*pow(t  , 2);
    float b = 1-undershoot*pow(t-1, 2);
    return mix(a, b, smoothstep(0, 1, t));
}

float tanh(float x)
{
    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}

float max_component(vec2 v)
{
    return max(v.x, v.y);
}

float sin01(float x)
{
    return 0.5*sin(2*PI*x)+0.5;
}