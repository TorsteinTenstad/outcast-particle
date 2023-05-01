#version 120

#include "shaders\\include\\standard_uniforms.glsl";

uniform float grid_width;
uniform float grid_height;

bool IsWallAt(vec2 gc_id)
{
	if (gc_id.x < 0 || gc_id.y < 0 || gc_id.x >= grid_width || gc_id.y >= grid_height)
	{
		return true;
	}
	return texture2D(_texture, (gc_id + vec2(0.5, 0.5)) / vec2(grid_width, grid_height)).r > 0.5;
}

float GAt(vec2 gc_id)
{
	if (gc_id.x < 0 || gc_id.y < 0 || gc_id.x >= grid_width || gc_id.y >= grid_height)
	{
		return 0;
	}
	return texture2D(_texture, (gc_id + vec2(0.5, 0.5)) / vec2(grid_width, grid_height)).g;
}

vec3 ColorOf(float g_val)
{
	if (g_val < 0.1)
	{
		return vec3(0.3);
	}
	if (g_val < 0.6)
	{
		return vec3(0.45);
	}
	return vec3(0.5, 0.8, 0.5);
}
float DepthOf(float g_val)
{
	if (g_val < 0.1)
	{
		return 0.2;
	}
	if (g_val < 0.6)
	{
		return 0.4;
	}
	return 0.8;
}

#define FAR_AWAY 1

void main()
{
	vec2 c = vec2(grid_width, grid_height) * vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
	vec2 g_id = floor(c);
	vec2 gc = fract(c);

	float dist_from_edge = FAR_AWAY;
	if (!IsWallAt(g_id + vec2(-1, 0)))
	{
		dist_from_edge = min(dist_from_edge, gc.x);
	}
	if (!IsWallAt(g_id + vec2(+1, 0)))
	{
		dist_from_edge = min(dist_from_edge, 1 - gc.x);
	}
	if (!IsWallAt(g_id + vec2(0, -1)))
	{
		dist_from_edge = min(dist_from_edge, gc.y);
	}
	if (!IsWallAt(g_id + vec2(0, +1)))
	{
		dist_from_edge = min(dist_from_edge, 1 - gc.y);
	}

	if (!IsWallAt(g_id + vec2(-1, -1)))
	{
		dist_from_edge = min(dist_from_edge, length(gc));
	}
	if (!IsWallAt(g_id + vec2(+1, +1)))
	{
		dist_from_edge = min(dist_from_edge, length(1 - gc));
	}
	if (!IsWallAt(g_id + vec2(-1, +1)))
	{
		dist_from_edge = min(dist_from_edge, length(vec2(gc.x, 1 - gc.y)));
	}
	if (!IsWallAt(g_id + vec2(+1, -1)))
	{
		dist_from_edge = min(dist_from_edge, length(vec2(1 - gc.x, gc.y)));
	}

	float val = smoothstep(0.2, 0.3, dist_from_edge / FAR_AWAY);
	gl_FragColor.rgb = mix(ColorOf(GAt(g_id)), vec3(0.1), val);
	gl_FragColor.a = IsWallAt(g_id) ? 1 : 0;
}