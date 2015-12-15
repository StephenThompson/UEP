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

// Values to pass to the fragment shader
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec3 vNormal;

uniform float shift;

void main() {
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
	vTextureCoord0 = vec2(gl_Vertex.x +shift,gl_Vertex.z +shift);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}