#version 120

// Constant across both shaders
uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform float BlendFactor = 0.65f;

// Values passed in from the vertex shader
varying vec3 vPosition;
varying vec2 vTextureCoord0;

void main() {
    vec4 texel0 = texture2D(texture0, vTextureCoord0*0.5f);
    vec4 mixed = mix(vec4(0.25f,0.6f,0.75f,1.0f),vec4(1.f,1.f,1.f,1.0f), max(0, min(1,(texel0.r-0.25f) * 2.f)));

    gl_FragColor = mix(mixed, vec4(15 / 255.f, 70 / 255.f, 126 / 255.f, 1.f), max(0, min(1, vPosition.z/4000.f)));
}