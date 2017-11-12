#pragma once
#include "DAY1.h"


class basicCube : public pipeline_state_t
{
	ID3D11Buffer* cubeVertexBuffer;
	ID3D11Buffer* cubeIndexBuffer;
	D3D11_BUFFER_DESC* cubeIndexBufferDesc;
	D3D11_BUFFER_DESC* cubeVertexBufferDesc;

	ID3D11DeviceContext* cubeDeviceContext;
	ID3D11Device* cubeDevice;
	ID3D11Resource* cubeResource;
	IDXGISwapChain * cubeSwapChain;

	//need to have a quick access way to get to the position of the cube to change it

	basicCube(XMFLOAT3 *color, float* sizeOfCube, XMFLOAT4* position);
	~basicCube();

	void createBuffers();
	unsigned int cubeIndices[8];

public:
};

