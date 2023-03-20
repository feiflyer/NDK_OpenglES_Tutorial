#version 300 es
precision mediump float;
precision mediump sampler3D;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform sampler3D textureLUT;
uniform float filterRatio;
out vec4 FragColor;
void main()
{
    vec4 color = texture(ourTexture, TexCoord);
    vec4 lutColor = texture(textureLUT, color.rgb);
    FragColor = mix(color, vec4(lutColor.rgb, lutColor.w), filterRatio);
}