#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <DirectXMath.h>
#pragma pack


struct SIMPLE_VERT
{
	//	XMFLOAT3 pos;
	float pos[4];
	float color[4];
	float uv[2]; 
	float normals[3];
	//float tangents[4];

} simpVert;

struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y, z;

};
struct light
{

	vec3 diffuse;
	vec3 emissive;
	vec3 ambient;
	vec3 specular;
	vec3 transparency;
	vec3 reflection;
};

struct vec4
{
	float x, y, z, w;

};
//int d = 0;
//
//(double)d;

 
#pragma pop
using namespace std;


class simpMaterial
{

public:
	
	vec3 diffuse;
	vec3 emissive;
	vec3 ambient;
	vec3 specular;
	vec3 transparency;
	vec3 reflection;
	 
	
	double shininess;
	void* texture;
	std::vector<const char*> path;
	std::vector<vec4> jointTransforms;

}customMaterial;

class simpMesh
{
public:

	vector<SIMPLE_VERT> vertData;
	vector<unsigned int> indices;

};