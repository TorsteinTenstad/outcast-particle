float get_rounded_corners_mask(vec2 xy, vec2 wh, float r)
{
    vec2 folded_coords = abs(xy-(wh)/2);
    vec2 r_smaller_box = min(folded_coords, vec2(wh)/2-r);
    vec2 dist_from_smaller_box = folded_coords-r_smaller_box;
    float alpha = 1-smoothstep(r-2, r+2, length(dist_from_smaller_box));
    return alpha;
}

float get_rounded_corners_alpha(vec2 xy, vec2 wh)
{
    float r = min(wh.x, wh.y)/4;
    return get_rounded_corners_mask(xy, wh, r);
}