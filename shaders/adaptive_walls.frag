#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\rand.glsl";
#include "shaders\\include\\math_utils.glsl";

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

//Should be syncronized with globals::WALL_BOUNCE_CATEGORIES
vec3 ColorOf(float g_val, float dist_from_edge)
{
	float bounce_factor = g_val*2-0.01; // Scaled in render_grid_adaptive_textures.cpp to handle values > 1, biased to make following thresholds reliable
	if (bounce_factor < 0.0)
	{
		return vec3(0.1);
	}
	if (bounce_factor < 0.05)
	{
		return vec3(0.3);
	}
	if (bounce_factor < 0.2)
	{
		return vec3(0.45);
	}
	if  (bounce_factor < 1.0)
	{
		return vec3(0.5, 0.8, 0.5);
	}
	return mix(vec3(0.2, 1, 0.2), vec3(0.2, 0.6, 1), sin01(3*dist_from_edge+1.5*_time));
}
float DepthOf(float g_val)
{
	float bounce_factor = g_val*2-0.01; // Scaled in render_grid_adaptive_textures.cpp to handle values > 1, biased to make following thresholds reliable
	if  (bounce_factor < 1.0)
	{
		return 0.2;
	}
	return 0.5;
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

	float g_val = GAt(g_id);
	float depth = 0.2;
	float transition_width = 0.05;
	float val = smoothstep(depth-transition_width, depth+transition_width, dist_from_edge / FAR_AWAY);
	gl_FragColor.rgb = mix(ColorOf(g_val, dist_from_edge), vec3(0.1), val);
	gl_FragColor.a = IsWallAt(g_id) ? 1 : 0;
}