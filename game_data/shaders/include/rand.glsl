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

float smooth_noise(vec2 uv) {
    vec2 lv = fract(uv);
    vec2 id = floor(uv);
    
    lv = lv*lv*(3.-2.*lv);
    
    float bl = rand01(id);
    float br = rand01(id+vec2(1,0));
    float b = mix(bl, br, lv.x);
    
    float tl = rand01(id+vec2(0,1));
    float tr = rand01(id+vec2(1,1));
    float t = mix(tl, tr, lv.x);
    
    return mix(b, t, lv.y);
}

float layered_smooth_noise(vec2 uv) {
    float c = 0;
    c += smooth_noise(uv* 2.0)*1.0;
    c += smooth_noise(uv* 4.2)*0.5;
    c += smooth_noise(uv* 8.7)*0.25;
    c += smooth_noise(uv*16.4)*0.125;
    c += smooth_noise(uv*32.5)*0.0625;
    c /= 2.;
    return c;
}