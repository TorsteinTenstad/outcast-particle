#version 120

#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

uniform vec2 _wh;
uniform vec2 width_and_height;
uniform vec2 position;
uniform float _time;
uniform sampler2D _noise_texture;
#define noise_size 1000
float rand01(vec2 uv)
{
	uv = fract(uv / noise_size);
	return texture2D(_noise_texture, 1 - uv).r;
}

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

mat2 rot(float theta)
{
	float s = sin(theta);
	float c = cos(theta);
	mat2 m = mat2(c, -s, s, c);
	return m;
}

float radial_falloff(float r, float a, float b)
{
	float f = 1 / (max((r - a), 0) / (b - a) + 1);
	return smoothstep(0.5, 1, f);
}

float twinkle(vec2 uv)
{
	return 0.01 / abs(uv.x * uv.y);
}

float twinkle(vec2 uv, float theta, float intensity)
{
	uv = rot(theta) * uv;
	return intensity * twinkle(uv) * radial_falloff(length(uv), 0, 0.5 + 0.5 * smoothstep(0, 1, intensity));
}

vec2 compute_path_animation(vec2 width_and_height, float t)
{
	float circumference = (2 * width_and_height.x + 2 * width_and_height.y);
	float w_len_fraction = width_and_height.x / circumference;
	float h_len_fraction = width_and_height.y / circumference;
	vec2 path_v = vec2(0, 0);
	path_v += width_and_height * vec2(+1, +0) * clamp(t - (0 * w_len_fraction + 0 * h_len_fraction), 0, w_len_fraction) / w_len_fraction;
	path_v += width_and_height * vec2(+0, +1) * clamp(t - (1 * w_len_fraction + 0 * h_len_fraction), 0, h_len_fraction) / h_len_fraction;
	path_v += width_and_height * vec2(-1, +0) * clamp(t - (1 * w_len_fraction + 1 * h_len_fraction), 0, w_len_fraction) / w_len_fraction;
	path_v += width_and_height * vec2(+0, -1) * clamp(t - (2 * w_len_fraction + 1 * h_len_fraction), 0, h_len_fraction) / h_len_fraction;
	return path_v;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec2 xy = uv*_wh;

	float absolute_animation_t = _time / 3;

	vec4 color = vec4(0);
	float total_twinkle = 0;
	int n = 5;
	for (int i = 0; i < n; ++i)
	{
		float p = float(i) / float(n);
		float animation_t = fract(absolute_animation_t - p);
		float animation_i = floor(absolute_animation_t - p);
		float path_t = fract((i + rand01(vec2(animation_i, i))) / n);
		vec2 path_v = compute_path_animation(width_and_height, path_t);
		vec2 twinkle_uv = 1.3 * (xy - position + width_and_height / 2.f - path_v) / width_and_height.y;
		float theta = 1.1 * _time * PI / 4.0;
		float intensity = clamp(0.5 * sin(2 * PI * (animation_t)), 0, 1);

		total_twinkle = max(total_twinkle, twinkle(twinkle_uv, theta, intensity));
	}
	gl_FragColor = vec4(vec3(1), total_twinkle);
}