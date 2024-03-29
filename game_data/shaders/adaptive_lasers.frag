#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";

uniform float grid_width;
uniform float grid_height;

vec4 TextureAt(vec2 gc_id)
{
	return texture2D(_texture, (gc_id + vec2(0.5, 0.5)) / vec2(grid_width, grid_height));
}

bool IsLaserAt(vec2 gc_id)
{
	if (gc_id.x < 0 || gc_id.y < 0 || gc_id.x >= grid_width || gc_id.y >= grid_height)
	{
		return true;
	}
	return TextureAt(gc_id).r > 0.5;
}
#define FAR_AWAY 1

void main()
{
	vec2 c = vec2(grid_width, grid_height) * vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	vec2 g_id = floor(c);
	vec2 gc = fract(c);

	bool b0 = IsLaserAt(g_id + vec2(-1, +1));
	bool b1 = IsLaserAt(g_id + vec2(-1, +0));
	bool b2 = IsLaserAt(g_id + vec2(-1, -1));
	bool b3 = IsLaserAt(g_id + vec2(+0, +1));
	bool b4 = IsLaserAt(g_id + vec2(+0, +0));
	bool b5 = IsLaserAt(g_id + vec2(+0, -1));
	bool b6 = IsLaserAt(g_id + vec2(+1, +1));
	bool b7 = IsLaserAt(g_id + vec2(+1, +0));
	bool b8 = IsLaserAt(g_id + vec2(+1, -1));

	float dist = FAR_AWAY;
	if (b0 && b1 && b3 && b4)
	{
		vec2 dist_corner = abs(gc - vec2(0, 1));
		dist = min(dist, length(dist_corner));
		if (b5 && b2)
		{
			dist = min(dist, dist_corner.x);
		}
		if (b7 && b6)
		{
			dist = min(dist, dist_corner.y);
		}
	}
	if (b1 && b2 && b4 && b5)
	{
		vec2 dist_corner = abs(gc - vec2(0, 0));
		dist = min(dist, length(dist_corner));
		if (b3 && b0)
		{
			dist = min(dist, dist_corner.x);
		}
		if (b7 && b8)
		{
			dist = min(dist, dist_corner.y);
		}
	}
	if (b4 && b5 && b7 && b8)
	{
		vec2 dist_corner = abs(gc - vec2(1, 0));
		dist = min(dist, length(dist_corner));
		if (b3 && b6)
		{
			dist = min(dist, dist_corner.x);
		}
		if (b1 && b2)
		{
			dist = min(dist, dist_corner.y);
		}
	}
	if (b3 && b4 && b6 && b7)
	{
		vec2 dist_corner = abs(gc - vec2(1, 1));
		dist = min(dist, length(dist_corner));
		if (b5 && b8)
		{
			dist = min(dist, dist_corner.x);
		}
		if (b1 && b0)
		{
			dist = min(dist, dist_corner.y);
		}
	}
	float intensity = radial_falloff(dist, 0.2, 0.9 + 0.1 * sin(0.5 * 2 * PI * _time));

	gl_FragColor.rgb = vec3(1, 0, 0) + vec3(1) * 0.5 * radial_falloff(dist, 0, 0.3);
	gl_FragColor.a = intensity*TextureAt(g_id).b;
}