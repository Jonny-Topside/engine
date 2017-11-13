#pragma once

#include "resource.h"
#include <d3d11.h> 
#include <DirectXMath.h> 
#include <DirectXColors.h> 
using namespace DirectX;

struct SEND_TO_VRAM
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct SIMPLE_VERTEX
{
	//whenver I change this structure, it affects the cube being drawn, I will have to change the input layout in conjunciton as well
	//unsigned int index;
	XMFLOAT3 pos;
	XMFLOAT3 color;
	XMFLOAT2 uvs;
	XMFLOAT3 normals;
};
struct triangle
{
	SIMPLE_VERTEX point[3];

};

class pipeline_state_t
{
public:

	ID3D11DeviceContext *context;
	ID3D11InputLayout *input_layout;
	ID3D11VertexShader *vertex_shader;
	ID3D11PixelShader * pixel_shader;
	ID3D11PixelShader * magePixelShader;


	IDXGISwapChain * m_pSwapChain;
	ID3D11Device * m_pDevice;
	ID3D11Resource * resource = nullptr;

	ID3D11Buffer* constBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* vertexBuffer2;
	ID3D11Buffer* constBuffer2;

	ID3D11Buffer* mageIndexBuffer;
	D3D11_BUFFER_DESC* constantBuffDesc;
	D3D11_BUFFER_DESC* constantBuffDesc2;
	D3D11_BUFFER_DESC* drawnBuffDesc;
	D3D11_BUFFER_DESC* drawnBuffDesc2;



	ID3D11RenderTargetView *render_target;
	ID3D11Texture2D *depthStencilBuffer = NULL;
	ID3D11DepthStencilState *depthStencilState;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11RasterizerState *rasterState;
	D3D11_VIEWPORT *vp;

}default_pipeline;