#pragma once

#include <DirectXMath.h> 

#include <d3d11.h> 
using namespace DirectX;

struct modelViewProjectionMatrix
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};
struct SIMPLE_VERTEX
{
	//properly align these as in the simple_vert

	XMFLOAT4 pos;
	XMFLOAT4 color;
	XMFLOAT2 uvs;
	XMFLOAT3 normals;
};
struct triangle
{
	SIMPLE_VERTEX point[3];

};
struct square
{
	triangle triangle[2];
};