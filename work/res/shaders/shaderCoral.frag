#version 120

// Constant across both shaders
uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform float BlendFactor = 0.65f;

// Values passed in from the vertex shader
varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTextureCoord0;

void main() {
    vec4 dirColor = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_FrontMaterial.diffuse * 
		    max(dot(vNormal, gl_LightSource[0].position.xyz), 0) * gl_LightSource[0].diffuse;
    vec4 texel0 = texture2D(texture0, vTextureCoord0*0.5f);
    vec4 mixed = mix(vec4(0.25f,0.6f,0.75f,1.0f),vec4(1.f,1.f,1.f,1.0f), texel0.r);
  
    gl_FragColor = mix(dirColor * mixed, vec4(15 / 255.f, 70 / 255.f, 126 / 255.f, 1.f), max(0, min(1, vPosition.z/2000.f)));
}