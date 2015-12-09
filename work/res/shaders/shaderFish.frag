//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
uniform sampler2D texture0;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;

void main() {
	//float light = 0.5f + max(0, dot(vec3(1,-1,1),vNormal))*0.5f;
	vec3 dirColor = vec4(gl_LightSource[0].ambient + max(dot(vNormal, gl_LightSource[0].position.xyz), 0) * gl_LightSource[0].diffuse).xyz;
	gl_FragColor.rgb = mix(dirColor * texture2D(texture0, vTextureCoord0).rgb, vec3(15 / 255.f, 70 / 255.f, 126 / 255.f), max(0, min(1, vPosition.z/6000.f)));//vTextureCoord0);
}