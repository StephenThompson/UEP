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
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec2 vTextureCoord1;

varying vec4 projectedPos;
varying vec4 curVertex;

uniform float shift;
uniform mat4 depthMVP;

void main() {
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewProjectionMatrix * gl_Vertex);
	projectedPos = vec4(depthMVP * gl_Vertex);
	vTextureCoord0 = vec2(gl_MultiTexCoord0.x+shift,gl_MultiTexCoord0.y+shift);
	vTextureCoord1 = gl_MultiTexCoord0.xy;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	curVertex = gl_Vertex;
}