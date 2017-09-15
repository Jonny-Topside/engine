#pragma once
#include "resource.h"
#include <d3d11.h>
#include "DAY1.h"
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h> 
#include <vector>
using namespace std;
using namespace DirectX;

struct line
{
	SIMPLE_VERTEX point[2];
};



struct cube
{
	line lines[12];
	SIMPLE_VERTEX simps[8];
};

class DebugRenderer : public pipeline_state_t
{
	const int maxVerts = 2048;
	unsigned int offset;
	unsigned int stride;
	int vertCount;
public:
	DebugRenderer();
	void init();
	~DebugRenderer();

	D3D11_MAPPED_SUBRESOURCE MappedSubresources;

	vector<SIMPLE_VERTEX> debugLines;
	ID3D11DeviceContext* context;
	//ID3D11Device* device;
	ID3D11Buffer* lineConstantBuffer;
	ID3D11Buffer* debugVertexBuffer;
	D3D11_BUFFER_DESC* lineConstantBuffDesc;
	D3D11_BUFFER_DESC vertexBufferDesc;
	void add_debug_line(SIMPLE_VERTEX a, SIMPLE_VERTEX b, XMFLOAT3 color);
	cube collisionLines(cube aabb);
	void clearVector();

	void flush();



};
