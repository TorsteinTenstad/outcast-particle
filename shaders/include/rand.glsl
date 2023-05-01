uniform sampler2D _noise_texture;

#define noise_size 1000

float rand01(vec2 uv)
{
	uv = fract(uv / noise_size);
	return texture2D(_noise_texture, 1 - uv).r;
}

float rand(float a, float b, vec2 seed)
{
	return a + rand01(seed) * (b - a);
}