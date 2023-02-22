#version 120

uniform sampler2D _texture;
uniform vec2 _wh;
uniform float grid_width;
uniform float grid_height;
uniform float grid_size;

bool IsWallAt(vec2 gc_id){
    if (gc_id.x < 0 || gc_id.y < 0 || gc_id.x>=grid_width || gc_id.y>=grid_height){
        return true;
    }
    return texture2D(_texture, (gc_id + vec2(0.5, 0.5))/vec2(grid_width, grid_height)).r>0.5;
}

float GAt(vec2 gc_id){
    if (gc_id.x < 0 || gc_id.y < 0 || gc_id.x>=grid_width || gc_id.y>=grid_height){
        return 0;
    }
    return texture2D(_texture, (gc_id + vec2(0.5, 0.5))/vec2(grid_width, grid_height)).g;
}

vec3 ColorOf(float g_val)
{
    return mix(vec3(0.8), vec3(0.6, 1, 0.6), (g_val));
}

#define FAR_AWAY 1

void main()
{
    vec2 c = vec2(grid_width, grid_height)*vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
    vec2 g_id = floor(c);
    vec2 gc = fract(c);

    float dist_from_edge = FAR_AWAY;
    if (!IsWallAt(g_id+vec2(-1,0))){
        dist_from_edge = min(dist_from_edge, gc.x);
    }
    if (!IsWallAt(g_id+vec2(+1,0))){
        dist_from_edge = min(dist_from_edge, 1-gc.x);
    }
    if (!IsWallAt(g_id+vec2(0,-1))){
        dist_from_edge = min(dist_from_edge, gc.y);
    }
    if (!IsWallAt(g_id+vec2(0,+1))){
        dist_from_edge = min(dist_from_edge, 1-gc.y);
    }

    if (!IsWallAt(g_id+vec2(-1,-1))){
        dist_from_edge = min(dist_from_edge, length(gc));
    }
    if (!IsWallAt(g_id+vec2(+1,+1))){
        dist_from_edge = min(dist_from_edge, length(1-gc));
    }
    if (!IsWallAt(g_id+vec2(-1,+1))){
        dist_from_edge = min(dist_from_edge, length(vec2(gc.x, 1-gc.y)));
    }
    if (!IsWallAt(g_id+vec2(+1,-1))){
        dist_from_edge = min(dist_from_edge, length(vec2(1-gc.x, gc.y)));
    }

    float val = smoothstep(0.1, 0.5, dist_from_edge/FAR_AWAY);
    gl_FragColor.rgb = mix(ColorOf(GAt(g_id)), vec3(0.1), val);
    gl_FragColor.a = IsWallAt(g_id) ? 1 : 0;
}