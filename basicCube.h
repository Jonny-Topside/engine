#pragma once
#define WIDTH 1080
#define HEIGHT 720
#include "DAY1.h"


class basicCube : public pipeline_state_t
{
	float FOV = 70.0f * XM_PI / 180.0f;
	float aspRatio = WIDTH / HEIGHT;
	unsigned int cubeIndices[8];

	ID3D11Buffer* cubeVertexBuffer;
	ID3D11Buffer* cubeIndexBuffer;
	ID3D11Buffer* cubeConstantBuffer;
	D3D11_BUFFER_DESC* cubeIndexBufferDesc;
	D3D11_BUFFER_DESC* cubeVertexBufferDesc;

	ID3D11DeviceContext* cubeDeviceContext;
	ID3D11Device* cubeDevice;
	ID3D11Resource* cubeResource;
	IDXGISwapChain * cubeSwapChain;

	//need to have a quick access way to get to the position of the cube to change it


	void createBuffers();
	void displayCube();

public:

	basicCube(XMFLOAT3 *color, float* sizeOfCube, XMFLOAT4* position);
	~basicCube();
};

