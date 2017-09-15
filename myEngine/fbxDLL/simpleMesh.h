#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#pragma pack
struct SIMPLE_VERT
{
	//	XMFLOAT3 pos;
	float pos[4];
	float color[4];
	//XMFLOAT3 color;
};

struct SIMPLE_MAT
{
	uint64_t id;
	const wchar_t* path;

};
#pragma pop
using namespace std;

//class simpMaterials
//{
//	//vector<> properties;
//};
class simpMaterial
{
public:
	uint64_t id;
	const wchar_t* path;
	

};

class simpMesh
{
public:

	vector<SIMPLE_VERT> vertData;
	vector<unsigned int> indices;

};