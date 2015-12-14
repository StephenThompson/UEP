#version 120

// Constant across both shaders
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float BlendFactor = 0.65f;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec2 vTextureCoord1;

varying vec4 projectedPos;
varying vec4 curVertex;



void main() {

	float brightness = 2.5f;

    vec4 texel1 = texture2D(texture1, vTextureCoord1*0.5f);
    vec4 texel0 = texture2D(texture0, vTextureCoord0*0.5f);

    vec4 blended = (texel0*(BlendFactor)+texel1*(1.f-BlendFactor));
    vec4 mixed = mix(texel0.rrrr,vec4(0.15f,0.5f,0.65f,1.0f),vec4(0.15f,0.5f,0.65f,1.0f));
    //gl_FragColor = blended*mixed*curVertex.y*brightness;
    
    gl_FragColor = mix(blended*mixed*(-curVertex.y/450.f)*brightness, vec4(15 / 255.f, 70 / 255.f, 126 / 255.f, 1.f)*0.9f, max(0, min(1, vPosition.z/1500.f - (-curVertex.y/450.f) / 5.f)));
    //gl_FragColor = blended*mixed*brightness*(-curVertex.y/450.f);
}
