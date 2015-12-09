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
uniform float time;
uniform float amount;
uniform float vertical;

// Values to pass to the fragment shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;

void main() {
	vec4 pos = gl_Vertex;
	if (vertical > 0.5f){
		pos.y += sin(pos.x - time)*amount;
	} else {
		pos.z += sin(pos.x - time)*amount;
	}
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewProjectionMatrix * pos);
	vTextureCoord0 = vec2(gl_MultiTexCoord0.x, -gl_MultiTexCoord0.y);
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}