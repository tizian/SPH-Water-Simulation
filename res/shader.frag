uniform sampler2D texture;

varying in vec4 gradientColor;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    if (pixel.r > 0.3f) {
        gl_FragColor = gradientColor;
    }
    else {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}