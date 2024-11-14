uniform float time;
uniform sampler2D texture;

void main() {
    vec2 pos = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, pos);

    float speedMultiplier = 4.0;
    
    float r = sin(speedMultiplier * time + pos.x * 10.0) * 0.5 + 0.5;
    float g = sin(speedMultiplier * time + pos.x * 10.0 + 2.0) * 0.5 + 0.5;
    float b = sin(speedMultiplier * time + pos.x * 10.0 + 4.0) * 0.5 + 0.5;
    
    gl_FragColor = vec4(r, g, b, 1.0) * color;
}
