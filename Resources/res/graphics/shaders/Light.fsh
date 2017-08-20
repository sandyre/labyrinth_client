#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 resolution;

void main(void)
{
    float cutoffRadius = 0.5;
    vec2 lightPos = vec2(0.5, 0.5);
    vec3 ambientColor = vec3(0.1, 0.1, 0.1);
    vec3 lightColor = vec3(1.0, 0.8, 0.8);
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    vec3 finalColor = texColor.rgb;

    vec2 midPoint = resolution * 0.5;
    vec2 posInPixels = resolution * v_texCoord;

    float radius = min(resolution.x, resolution.y) * cutoffRadius;
    float dist = length(posInPixels - midPoint);

    if (dist < radius)
    {
        finalColor = finalColor * (ambientColor + (lightColor * clamp(1.0 - (dist / radius), 0.01, 0.8) * (0.7 + 0.1 * sin(CC_Time.z))));
    }
    else
        finalColor = texColor.rgb * ambientColor;

    gl_FragColor = vec4(finalColor, texColor.a);
}
