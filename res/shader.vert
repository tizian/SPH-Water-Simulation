varying out vec4 gradientColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_FrontColor = gl_Color;

    vec4 lightBlue = vec4(0.36, 0.6, 1.0, 1.0);
    vec4 midnightBlue = vec4(0.0, 0.054, 0.144, 1.0);

    float alpha = (gl_Position.y + 1.0) * 2.0 ;

    gradientColor = mix(midnightBlue, lightBlue, alpha);
}