#version 120
uniform sampler2D currentTexture;
uniform float time;
#define PI 3.1415926535897932384626433832795
void main() {
    vec2 coord = gl_TexCoord[0].xy;
    if (time < 1){
        coord.x -= (0.5-coord.x)*sin(PI*time) * 0.2;
        coord.y -= (0.5-coord.y)*sin(PI*time) * 0.2;
    }
    vec4 pixel_color = texture2D(currentTexture, coord);
    gl_FragColor = pixel_color;
}
