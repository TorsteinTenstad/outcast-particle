#version 120

#define PI 3.1415926535897932384626433832795

// NOTE: Must be in sync with constants.hpp

uniform sampler2D data_texture;
uniform int BLOCK_SIZE;
uniform int WIDTH;
uniform int HEIGHT;
uniform float _time;


int GridEntityAtId(int x, int y){
    if (x < 0 || y < 0 || x>=WIDTH || y>=HEIGHT){
        return 0;
    }
    return int(floor(texture2D(data_texture, vec2(x, y)/vec2(WIDTH-1, HEIGHT-1)).r*256.0));
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

    float dist_from_edge = FAR_AWAY;

    if (GridEntityAtId(grid_x-1, grid_y) == 0){
        dist_from_edge = min(dist_from_edge, local_coords.x);
    }
    if (GridEntityAtId(grid_x+1, grid_y) == 0){
        dist_from_edge = min(dist_from_edge, 1-local_coords.x);
    }
    if (GridEntityAtId(grid_x, grid_y-1) == 0){
        dist_from_edge = min(dist_from_edge, local_coords.y);
    }
    if (GridEntityAtId(grid_x, grid_y+1) == 0){
        dist_from_edge = min(dist_from_edge, 1-local_coords.y);
    }

    if (GridEntityAtId(grid_x-1, grid_y-1) == 0){
        dist_from_edge = min(dist_from_edge, length(local_coords));
    }
    if (GridEntityAtId(grid_x+1, grid_y+1) == 0){
        dist_from_edge = min(dist_from_edge, length(1-local_coords));
    }
    if (GridEntityAtId(grid_x-1, grid_y+1) == 0){
        dist_from_edge = min(dist_from_edge, length(vec2(local_coords.x, 1-local_coords.y)));
    }
    if (GridEntityAtId(grid_x+1, grid_y-1) == 0){
        dist_from_edge = min(dist_from_edge, length(vec2(1-local_coords.x, local_coords.y)));
    }

    float val = smoothstep(0, 0.3, dist_from_edge/FAR_AWAY);
    vec3 col = mix(vec3(0.9), vec3(0.1), val);


    int entity_type = GridEntityAtId(grid_x, grid_y);
	gl_FragColor = vec4(col, float(entity_type));
	//gl_FragColor = vec4(gl_TexCoord[0].xy/120, 0, 1);
}