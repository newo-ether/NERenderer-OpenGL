#version 330 core

out vec4 outColor;

uniform sampler2D lastRenderResult;
uniform uvec2 screenGeometry;
uniform float gamma;

void main() {
    vec3 renderColor = texture2D(lastRenderResult, gl_FragCoord.xy / vec2(screenGeometry)).xyz;
    outColor = vec4(clamp(pow(renderColor.x, 1.0f / gamma), 0.0f, 1.0f),
                    clamp(pow(renderColor.y, 1.0f / gamma), 0.0f, 1.0f),
                    clamp(pow(renderColor.z, 1.0f / gamma), 0.0f, 1.0f),
                    1.0f);
}
