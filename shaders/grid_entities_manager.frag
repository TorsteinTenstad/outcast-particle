#version 120

#define PI 3.1415926535897932384626433832795

// NOTE: Must be in sync with constants.hpp

uniform sampler2D data_texture;
uniform int BLOCK_SIZE;
uniform int TEXTURE_WIDTH;
uniform int TEXTURE_HEIGHT;
uniform int level_width;
uniform int level_height;
uniform float _time;


float radial_falloff(float r, float r1, float r0){
	float f = 1/(max((r-r1), 0)/r0+1);
	return smoothstep(0.5, 1, f);
}

int GridEntityAtId(int x, int y, int outside){
    if (x < 0 || y < 0 || x>=level_width || y>=level_height){
        return outside;
    }
    return int(floor(texture2D(data_texture, vec2(x, y)/vec2(TEXTURE_WIDTH-1, TEXTURE_HEIGHT-1)).r*256.0));
}

#define SQRT2 1.41421356
#define FAR_AWAY 1

void main()
{
    vec2 grid_coords= gl_TexCoord[0].xy/BLOCK_SIZE;

	vec2 grid_id = floor(grid_coords);
    int grid_x = int(grid_id[0]);
    int grid_y = int(grid_id[1]);
	vec2 local_coords = fract(grid_coords);
    vec2 centered_local_coords = local_coords - 0.5;
    vec2 quadrant = 2*ceil(centered_local_coords)-1;
    vec2 folded_local_coords = abs(centered_local_coords);
        
    vec4 color = vec4(1, 0, 1, 1);

    int entity_type = GridEntityAtId(grid_x, grid_y, 0);
    switch (entity_type){
    case 0:
        color.a = 0;
        break;
    case 1:
        float r = 0.15;
        vec3 edge_color = vec3(0.8);
        vec2 rounding_vector = folded_local_coords-(0.5-r);
        if(length(rounding_vector) > r
            && rounding_vector.x > 0
            && rounding_vector.y > 0
            && GridEntityAtId(grid_x + int(quadrant.x), grid_y, 1) != 1
            && GridEntityAtId(grid_x, grid_y + int(quadrant.y), 1)!=1){
            gl_FragColor = vec4(edge_color, smoothstep(r+0.02, r, length(rounding_vector)));
            return;
        }
        float dist_from_edge = FAR_AWAY;
        if (GridEntityAtId(grid_x-1, grid_y, 1) != 1){
            dist_from_edge = min(dist_from_edge, local_coords.x);
        }
        if (GridEntityAtId(grid_x+1, grid_y, 1) != 1){
            dist_from_edge = min(dist_from_edge, 1-local_coords.x);
        }
        if (GridEntityAtId(grid_x, grid_y-1, 1) != 1){
            dist_from_edge = min(dist_from_edge, local_coords.y);
        }
        if (GridEntityAtId(grid_x, grid_y+1, 1) != 1){
            dist_from_edge = min(dist_from_edge, 1-local_coords.y);
        }

        if (GridEntityAtId(grid_x-1, grid_y-1, 1) != 1){
            dist_from_edge = min(dist_from_edge, length(local_coords));
        }
        if (GridEntityAtId(grid_x+1, grid_y+1, 1) != 1){
            dist_from_edge = min(dist_from_edge, length(1-local_coords));
        }
        if (GridEntityAtId(grid_x-1, grid_y+1, 1) != 1){
            dist_from_edge = min(dist_from_edge, length(vec2(local_coords.x, 1-local_coords.y)));
        }
        if (GridEntityAtId(grid_x+1, grid_y-1, 1) != 1){
            dist_from_edge = min(dist_from_edge, length(vec2(1-local_coords.x, local_coords.y)));
        }

        float val = smoothstep(0, 0.3, dist_from_edge/FAR_AWAY);
        color.rgb = mix(edge_color, vec3(0.1), val);
        break;
    case 2:
        int closest_other_entity_x = GridEntityAtId(grid_x + int(quadrant.x), grid_y, 0);
        int closest_other_entity_y = GridEntityAtId(grid_x, grid_y + int(quadrant.y), 0);
        
        float x_limit = float(closest_other_entity_x!=0)*0.5;
        vec2 closest_laser_x = vec2(clamp(folded_local_coords.x, 0, x_limit), 0);

        float y_limit = float(closest_other_entity_y!=0)*0.5;
        vec2 closest_laser_y = vec2(0, clamp(folded_local_coords.y, 0, y_limit));

        vec2 dist_x = folded_local_coords-closest_laser_x;
        vec2 dist_y = folded_local_coords-closest_laser_y;
        float dist = min(length(dist_x), length(dist_y));

        float intensity = radial_falloff(dist, 0, 0.5+ 0.05*sin(0.5*2*PI*_time));

        color.rgb = vec3(1,0,0) + vec3(1)*0.5*radial_falloff(dist, 0, 0.1);
        color.a = intensity;
    }
    gl_FragColor = color;
}