#include "shape.hpp"
#include "mainheaders.hpp"

Shape::Shape(vec3 position, float radius, bool hollow){
	m_position = position;
	m_radius = radius;
	m_hollow = hollow;
}

Shape::Shape(vec3 position, vec3 size, bool hollow){
	m_position = position;
	m_size = size;
	m_hollow = hollow;
}

vec3 Shape::randPoint(){
	if(m_radius>0){
		//Sphere
		float len = ((noise(seed)+1)/2)*m_radius;
		if(m_hollow){
		  len = m_radius;
		}
		float x = noise(seed)*m_radius;
		float y = noise(seed)*m_radius;
		float z = noise(seed)*m_radius;
		vec3 direction = normalize(vec3(x,y,z));
		return m_position+(direction*len);
	}else if(m_radius<0 && !m_hollow){
		//rect
		float xPoint = noise(seed)*m_size.x+m_position.x;
		float yPoint = noise(seed)*m_size.y+m_position.y;
		float zPoint = noise(seed)*m_size.z+m_position.z;
		return vec3(xPoint, yPoint, zPoint);
	}
	return vec3(0,0,0);
}

float Shape::noise(int x){
	x+=seed;
	seed+=1;
	x = (x<<13)^x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
