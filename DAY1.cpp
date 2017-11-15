// DAY1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "fbxDLL\fbxDLL.h"
#include "DAY1.h"
#include <bitset>
#include <ctime>
#include "EngineMath.h"
#include <iostream>
#include <Windows.h>
#include <d3d11.h> 
#include <chrono>

#include "DebugRenderer.h"
#include <DirectXColors.h>
#include "DDSTextureLoader.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <strstream>
#include "PixelShader.csh"
#include "magePixelShader.csh"
#include "VertexShader.csh"
#include "basicCube.h"
#include <stdlib.h>  
#include <crtdbg.h>  
#define _CRTDBG_MAP_ALLOC  
#define MAX_LOADSTRING 100
#define WIDTH 1080
#define HEIGHT 720
#define CYAN 0x00ffff
#define RED 0xff0000
#define MAGENTA 0xff00ff																																																																																	//Never let it be said that you couldn't do it Jonathan, you are capable of so much more than you think 4:58 AM
#define GREEN 0x00ff00
#define YELLOW 0xffff00
#define BLUE 0x0000ff
using namespace DirectX;

// Global Variables:
float lookrot = 0.0f;
float rotation = 0;
float looker = 0;
float rotscale = 0.5f;
bool displayStats = true;
int currentFrame;
int jumper;
bool buttonHit = false;
bool keyPressed = true;
bool gotten = false;
ID3D11InputLayout *float4_input_layout;
ID3D11InputLayout *layoutForUvs;
simpMesh terrain;
simpMaterial matText;
ID3D11Buffer* meshBuff = nullptr;
ID3D11Buffer* indexBuff;
ID3D11Texture2D* mageDiffuseTexture;
ID3D11Texture2D* mageEmissiveTexture;
ID3D11Texture2D* mageSpecularTexture;

vector<SIMPLE_VERTEX> quickTransform;


ID3D11SamplerState* mageDiffuseSS;
ID3D11SamplerState* mageEmissiveSS;
ID3D11SamplerState* mageSpecularSS;

ID3D11ShaderResourceView* mageDiffuseSRV;
ID3D11ShaderResourceView* mageEmissiveSRV;
ID3D11ShaderResourceView* mageSpecularSRV;
ID3D11Buffer* mageLightConstantBuffer;

triangle cubeFaceTriangles[12];
SIMPLE_VERTEX cubeVerts[8];

//simpMaterial matText;

HRESULT h;
float jonTimer = 0.0f;
SEND_TO_VRAM toShader;
SEND_TO_VRAM toShader2;
D3D11_SUBRESOURCE_DATA constantSrd;
D3D11_SUBRESOURCE_DATA constantSrd2;


//view projection model



float aspRatio = WIDTH / HEIGHT;
float FOV = 70.0f * XM_PI / 180.0f;
float zNear = 0.1f;
float nearPlane = 0.1f;
float farPlane = 10.0f;

float zFar = 10.0f;


//FORWARD DECS
// Forward declarations of functions included in this code module:




ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void UpdateMouse(float deltaTime, float x, float y);
BOOL                InitInstance(HINSTANCE, int);
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
class pipeline_state_t;
void drawCurrFrame();
HINSTANCE hInst;                                // current instance
HWND hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DAY1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
#ifndef NDEBUG 
	AllocConsole();
	FILE* new_std_in_out;
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
	std::cout << "Hello world!\n";
#endif
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAY1));

	MSG msg;

	//// Main message loop:
	//while (GetMessage(&msg, nullptr, 0, 0))
	//{
	//    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//    {
	//        TranslateMessage(&msg);
	//        DispatchMessage(&msg);
	//    }
	//}
	// Main application loop:

	std::chrono::time_point<std::chrono::system_clock> lastnow;
	lastnow = std::chrono::system_clock::now();
	float time = 0, deltaTime = 0;
	float fpsUpdate = 0;
	int counter = 0;
	float time2 = 0;
	bool running = true;
	while (running)
	{
		deltaTime = (float)(std::chrono::system_clock::now() - lastnow).count() / 1e7f;
		time += deltaTime;
		lastnow = std::chrono::system_clock::now();



		// Process all messages, stop on WM_QUIT
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			//_CrtSetBreakAlloc(18);
			_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
			// WM_QUIT does not need to be
			// translated or dispatched
			if (msg.message == WM_QUIT)
			{
				running = false;
				break;
			}
			// Translates messages and sends them
			// to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (running == false)
			break;

		if (GetKeyState(VK_RBUTTON) < 0)
		{
			static float mouseX = 0, mouseY = 0;
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(hWnd, &point);
			mouseX = ((float)(point.x) / WIDTH - 0.5f);
			mouseY = ((float)(point.y) / HEIGHT - 0.5f);
			UpdateMouse(deltaTime, mouseX, mouseY);
			point.x = WIDTH / 2;
			point.y = HEIGHT / 2;
			ClientToScreen(hWnd, &point);
			SetCursorPos(point.x, point.y);
		}
		 


		drawCurrFrame();
		// In the future, do per frame/tick updates here...
	}
#ifndef NDEBUG 
	FreeConsole();
#endif 
	return 0;
}



void UpdateMouse(float deltaTime, float x, float y)
{
	static float roty = 0, rotx = 0;
	static float lookX = 0, lookY = 0, lookZ = 0;
	const float scaler = 5.0f;
	const float bump = deltaTime * scaler;
	roty -= x;
	rotx -= y;
	if (rotx >= XM_PI / 2)
		rotx = XM_PI / 2 - 0.001f;
	if (rotx <= -XM_PI / 2)
		rotx = -XM_PI / 2 + 0.001f;

	if (GetAsyncKeyState('W')) {
		lookX += cos(roty)*cos(rotx)*bump;
		lookZ += sin(roty)*cos(rotx)*bump;
		lookY += sin(rotx)*bump;
	}
	if (GetAsyncKeyState('A')) {
		lookZ += cos(roty)*bump;
		lookX -= sin(roty)*bump;
	}
	if (GetAsyncKeyState('S')) {
		lookX -= cos(roty)*cos(rotx)*bump;
		lookZ -= sin(roty)*cos(rotx)*bump;
		lookY -= sin(rotx)*			bump;
	}
	if (GetAsyncKeyState('D')) {
		lookZ -= cos(roty)*bump;
		lookX += sin(roty)*bump;
	}



	const XMVECTORF32 EyePosition = { lookX, lookY, lookZ, 0.f };
	const XMVECTORF32 FocusPosition = { cos(roty)*cos(rotx) + lookX, sin(rotx) + lookY, sin(roty)*cos(rotx) + lookZ, 0.f };
	const XMVECTORF32 UpDirection = { .0f, 1.f, 0.f, 0.f };
	XMMATRIX LookAt = XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);





	XMStoreFloat4x4(&toShader.view, XMMatrixTranspose(LookAt));

	XMStoreFloat4x4(&toShader2.view, XMMatrixTranspose(LookAt));


}

void initVP()
{
	D3D11_VIEWPORT view;
	ZeroMemory(&view, sizeof(D3D11_VIEWPORT));
	view.Width = WIDTH;
	view.Height = HEIGHT;
	view.TopLeftX = 0;
	view.TopLeftY = 0;
	view.MinDepth = 0;
	view.MaxDepth = 1;
	default_pipeline.vp = &view;
	default_pipeline.context->RSSetViewports(1, default_pipeline.vp);
}

void setupTriangles()
{

	{
		//0
		cubeVerts[0].pos.x = -0.25f;
		cubeVerts[0].pos.y = -0.25f;		//BOTTOM LEFT FRONT 1
		cubeVerts[0].pos.z = -0.25f;
		//1		    	  
		cubeVerts[1].pos.x = -0.25f;
		cubeVerts[1].pos.y = -0.25f;		//BOTTOM LEFT BACK 2
		cubeVerts[1].pos.z = 0.25f;
		//2		    	  
		cubeVerts[2].pos.x = 0.25f;
		cubeVerts[2].pos.y = -0.25f;		//BOTTOM RIGHT FRONT 3
		cubeVerts[2].pos.z = -0.25f;
		//3	    		  
		cubeVerts[3].pos.x = 0.25f;
		cubeVerts[3].pos.y = -0.25f;		//BOTTOM RIGHT BACK 4
		cubeVerts[3].pos.z = 0.25f;
		//4		    	  
		cubeVerts[4].pos.x = -0.25f;
		cubeVerts[4].pos.y = 0.25f;		// TOP LEFT FRONT 5
		cubeVerts[4].pos.z = -0.25f;
		//5	    		  
		cubeVerts[5].pos.x = -0.25f;
		cubeVerts[5].pos.y = 0.25f;		//TOP LEFT BACK 6
		cubeVerts[5].pos.z = 0.25f;
		//6	    		  
		cubeVerts[6].pos.x = 0.25f;
		cubeVerts[6].pos.y = 0.25f;		// TOP RIGHT BACK 7 
		cubeVerts[6].pos.z = 0.25f;
		//7	    			  
		cubeVerts[7].pos.x = 0.25f;
		cubeVerts[7].pos.y = 0.25f;		//TOP RIGHT FRONT 8
		cubeVerts[7].pos.z = -0.25f;
	}

	{

		//FACE 1 TRIANGLE 1 -Z
		//1

		cubeVerts[0].uvs = { 0,1 };
		cubeVerts[4].uvs = { 0,0 };
		cubeVerts[7].uvs = { 1,0 };
		cubeVerts[2].uvs = { 1,1 };

		cubeFaceTriangles[0].point[0] = cubeVerts[0];
		 
		cubeFaceTriangles[0].point[1] = cubeVerts[4]; //0,0 for uvs starts here
													  //cubeFaceTriangles[0].point[1].uvs = { 0,0 };

		cubeFaceTriangles[0].point[2] = cubeVerts[7];
		 
		//2											

		//just have enough uvs for the cube like 8
		cubeFaceTriangles[1].point[0] = cubeVerts[0]; // 0,1 for uvs is here
		cubeFaceTriangles[1].point[1] = cubeVerts[7]; // 1,0 for uvs is here
		cubeFaceTriangles[1].point[2] = cubeVerts[2]; // 1,1 for uvs is here

													  //since its fbx, use v = 1 - v for one of them
		for (unsigned int i = 0; i < 2; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 1.0f, 1.0f, 0.0f }; //yellow
			}
		}


		cubeVerts[7].uvs = { 0,0 };
		cubeVerts[2].uvs = { 0,1 };
		cubeVerts[6].uvs = { 1,0 };
		cubeVerts[3].uvs = { 1,1 };
		//FACE 2 TRIANGLE 3 +X	
		//3
		cubeFaceTriangles[2].point[0] = cubeVerts[2];
		cubeFaceTriangles[2].point[1] = cubeVerts[7]; // good
		cubeFaceTriangles[2].point[2] = cubeVerts[6];
		//4										
		cubeFaceTriangles[3].point[0] = cubeVerts[2];
		cubeFaceTriangles[3].point[1] = cubeVerts[6];
		cubeFaceTriangles[3].point[2] = cubeVerts[3];
		for (unsigned int i = 2; i < 4; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 1.0f, 0.0f, 0.0f }; //RED
			}
		}

		//FACE 3 TRIANGLE 5 +Z
		//5
		cubeVerts[6].uvs = { 0,0 };
		cubeVerts[3].uvs = { 0,1 };
		cubeVerts[1].uvs = { 1,1 };
		cubeVerts[5].uvs = { 1,0 };
		cubeFaceTriangles[4].point[0] = cubeVerts[3];
		cubeFaceTriangles[4].point[1] = cubeVerts[6];
		cubeFaceTriangles[4].point[2] = cubeVerts[5];
		//6										
		cubeFaceTriangles[5].point[0] = cubeVerts[3];
		cubeFaceTriangles[5].point[1] = cubeVerts[5];
		cubeFaceTriangles[5].point[2] = cubeVerts[1];
		for (unsigned int i = 4; i < 6; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 0.0f, 0.0f, 1.0f }; //blu
			}
		}


		//FACE 4 TRIANGLE 7 -X
		//7
		cubeVerts[5].uvs = { 0,0 };
		cubeVerts[4].uvs = { 1,0 };
		cubeVerts[1].uvs = { 0,1 };
		cubeVerts[0].uvs = { 1,1 };
		cubeFaceTriangles[6].point[0] = cubeVerts[1];
		cubeFaceTriangles[6].point[1] = cubeVerts[5];
		cubeFaceTriangles[6].point[2] = cubeVerts[4];
		//8										
		cubeFaceTriangles[7].point[0] = cubeVerts[1];
		cubeFaceTriangles[7].point[1] = cubeVerts[4];
		cubeFaceTriangles[7].point[2] = cubeVerts[0];
		for (unsigned int i = 6; i < 8; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 0.0f, 1.0f, 1.0f }; //CYAN
			}
		}//FACE 5 TRIANGLE 9 +Y
		 //9
		cubeVerts[5].uvs = { 0,0 };
		cubeVerts[4].uvs = { 0,1 };
		cubeVerts[7].uvs = { 1,1 };
		cubeVerts[6].uvs = { 1,0 };
		cubeFaceTriangles[8].point[0] = cubeVerts[4];
		cubeFaceTriangles[8].point[1] = cubeVerts[5];
		cubeFaceTriangles[8].point[2] = cubeVerts[6];
		//10
		cubeFaceTriangles[9].point[0] = cubeVerts[4];
		cubeFaceTriangles[9].point[1] = cubeVerts[6];
		cubeFaceTriangles[9].point[2] = cubeVerts[7];
		for (unsigned int i = 8; i < 10; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 0.0f, 1.0f, 0.0f }; //GREEN
			}
		}
		//FACE 6 TRIANGLE 11 -Y
		//11
		cubeVerts[1].uvs = { 0,0 };
		cubeVerts[0].uvs = { 0,1 };
		cubeVerts[2].uvs = { 1,1 };
		cubeVerts[3].uvs = { 1,0 };
		cubeFaceTriangles[10].point[0] = cubeVerts[0];
		cubeFaceTriangles[10].point[1] = cubeVerts[1];
		cubeFaceTriangles[10].point[2] = cubeVerts[3];
		//12
		cubeFaceTriangles[11].point[0] = cubeVerts[0];
		cubeFaceTriangles[11].point[1] = cubeVerts[3];
		cubeFaceTriangles[11].point[2] = cubeVerts[2];
		for (unsigned int i = 10; i < 12; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 1.0f, 0.0f, 1.0f }; //MAGENTA
			}
		}

	}


}

void initBuffer()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.ByteWidth = sizeof(triangle) * 12;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA srd = { cubeFaceTriangles, 0, 0 };
	default_pipeline.drawnBuffDesc = &vertexBufferDesc;
	default_pipeline.m_pDevice->CreateBuffer(default_pipeline.drawnBuffDesc, &srd, &default_pipeline.vertexBuffer);
	default_pipeline.m_pDevice->CreateVertexShader(VertexShader, sizeof(VertexShader), nullptr, &default_pipeline.vertex_shader);
	default_pipeline.m_pDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, &default_pipeline.pixel_shader);
	default_pipeline.m_pDevice->CreatePixelShader(magePixelShader, sizeof(magePixelShader), nullptr, &default_pipeline.magePixelShader);

}

void initDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	//CD3D11_TEXTURE2D_DESC cdTextureDesc;

	//cdTextureDesc = CD3D11_TEXTURE2D_DESC();
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	default_pipeline.m_pDevice->CreateTexture2D(&textureDesc, NULL, &default_pipeline.depthStencilBuffer);

	//set pipelines stages means the shaders and the buffers along with any other stuff at the end


}

void initDepthStencilState()
{
	CD3D11_DEPTH_STENCIL_DESC stateDesc;
	stateDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	stateDesc.StencilEnable = true;
	stateDesc.StencilReadMask = 0xFF;
	stateDesc.StencilWriteMask = 0xFF;
	stateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	stateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

	default_pipeline.m_pDevice->CreateDepthStencilState(&stateDesc, &default_pipeline.depthStencilState);
}

void initDepthStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));
	//stencilDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
	stencilDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	h = default_pipeline.m_pDevice->CreateDepthStencilView(default_pipeline.depthStencilBuffer, &stencilDesc, &default_pipeline.depthStencilView);


}





void initDevice()
{
	//pipeline_state_t* device = new pipeline_state_t;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = WIDTH;
	swapDesc.BufferDesc.Height = HEIGHT;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	h = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		&featureLevel, 1, D3D11_SDK_VERSION,
		&swapDesc, &default_pipeline.m_pSwapChain, &default_pipeline.m_pDevice, NULL,
		&default_pipeline.context);
	default_pipeline.context->OMSetRenderTargets(1, &default_pipeline.render_target, nullptr);
	default_pipeline.m_pSwapChain->GetBuffer(NULL, __uuidof(default_pipeline.resource),
		reinterpret_cast<void**>(&default_pipeline.resource));
	default_pipeline.m_pDevice->CreateRenderTargetView(default_pipeline.resource, nullptr, &default_pipeline.render_target);

	//this is more of a format, not the actual data
	//this is a promise that data in this format will be sent up(like a struct)
	//also the way to do this is to make structs with data and use constant buffers to send them 
	//to the shaders
	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		//	{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	default_pipeline.m_pDevice->CreateInputLayout(vLayout, ARRAYSIZE(vLayout),
		VertexShader, sizeof(VertexShader), &default_pipeline.input_layout);

	D3D11_INPUT_ELEMENT_DESC v4Layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	default_pipeline.m_pDevice->CreateInputLayout(v4Layout, ARRAYSIZE(v4Layout),
		VertexShader, sizeof(VertexShader), &float4_input_layout);

	D3D11_INPUT_ELEMENT_DESC layoutForIfThereAreUvs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	default_pipeline.m_pDevice->CreateInputLayout(layoutForIfThereAreUvs, ARRAYSIZE(layoutForIfThereAreUvs),
		VertexShader, sizeof(VertexShader), &layoutForUvs);


	//FIRST CONST BUFFER
	ZeroMemory(&constantSrd, sizeof(constantSrd));
	constantSrd.pSysMem = &toShader;

	D3D11_BUFFER_DESC constBuffDesc;
	constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBuffDesc.ByteWidth = sizeof(SEND_TO_VRAM);
	constBuffDesc.StructureByteStride = 0;
	constBuffDesc.MiscFlags = 0;
	HRESULT res = default_pipeline.m_pDevice->CreateBuffer(&constBuffDesc, &constantSrd, &default_pipeline.constBuffer);

	//SECOND CONST BUFFER
	ZeroMemory(&constantSrd2, sizeof(constantSrd2));
	constantSrd2.pSysMem = &toShader2;

	D3D11_BUFFER_DESC constBuffDesc2;
	constBuffDesc2.Usage = D3D11_USAGE_DYNAMIC;
	constBuffDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuffDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBuffDesc2.ByteWidth = sizeof(SEND_TO_VRAM);
	constBuffDesc2.StructureByteStride = 0;
	constBuffDesc2.MiscFlags = 0;
	default_pipeline.m_pDevice->CreateBuffer(&constBuffDesc2, &constantSrd2, &default_pipeline.constBuffer2);

	const XMVECTORF32	EyePosition = { -1.45f, -50, -50, 1.0f };
	const XMVECTORF32 FocusPosition = { cos(10)*cos(10) + 1, sin(10) + 1, sin(10)*cos(10) + 1, 0.f };
	const XMVECTORF32	UpDirection = { .0f, 1.f, 0.f, 0.f };


	XMMATRIX LookAt = XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);

	const XMVECTORF32	EyePosition2 = { -1.45f, -50, -50, 1.0f };
	const XMVECTORF32 FocusPosition2 = { cos(10.0f)*cos(10.0f) + 1.0f, sin(10.0f) + 1.0f, sin(10.0f)*cos(10.0f) + 1.0f, 0.0f };
	const XMVECTORF32	UpDirection2 = { .0f, 1.f, 0.f, 0.f };


	XMMATRIX LookAt2 = XMMatrixLookAtLH(EyePosition2, FocusPosition2, UpDirection2);


	XMStoreFloat4x4(&toShader.view, LookAt);
	XMStoreFloat4x4(&toShader2.model, XMMatrixTranspose(XMMatrixTranslation(2, 0, 0)));

	XMStoreFloat4x4(&toShader2.view, LookAt2);

}




void initRasterizer()
{
	CD3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	default_pipeline.m_pDevice->CreateRasterizerState(&rasterDesc, &default_pipeline.rasterState);

}

void setPipeline(pipeline_state_t* state)
{

	state->context->OMSetDepthStencilState(state->depthStencilState, 1);
	state->context->OMSetRenderTargets(1, &state->render_target, state->depthStencilView);
	state->context->RSSetState(state->rasterState);
	state->context->VSSetShader(state->vertex_shader, 0, 0);
	state->context->PSSetShader(state->pixel_shader, 0, 0);
	state->context->IASetInputLayout(state->input_layout);


}

void initAll()
{
	initDevice();
	initDepthStencilBuffer();
	initDepthStencilState();
	initDepthStencilView();
	initRasterizer();
	initVP();
	setupTriangles();
	initBuffer();
	setPipeline(&default_pipeline);

}

 
float upOne = 3.0f;

//void drawACube(XMFLOAT4X4 placeToDraw, )
 
void drawCurrFrame()
{
	DebugRenderer* debugRender = new DebugRenderer;

	jonTimer += 0.11f;
	const float newColor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	unsigned int offset = 0;
	unsigned int stride = sizeof(SIMPLE_VERTEX);
	 
	XMStoreFloat4x4(&toShader.model, XMMatrixIdentity());


	XMMATRIX projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(FOV, aspRatio, 0.01f, 100.0f));
	XMStoreFloat4x4(&toShader.projection, projection);
	//clears the screen to a certain color
	default_pipeline.context->ClearRenderTargetView(default_pipeline.render_target, newColor);
	default_pipeline.context->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	 
	////it wont work with xmfloat4; 
	
	//DRAW ROTATING CUBE
	//changing the model of the cube to rotate by a time  variable
	XMStoreFloat4x4(&toShader.model, XMMatrixMultiplyTranspose(XMLoadFloat4x4(&toShader.model), XMMatrixMultiply(XMMatrixRotationX(.1*jonTimer), XMMatrixRotationZ(.1*jonTimer))));
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;

	//updates model to rotate stuff
	default_pipeline.context->Map(default_pipeline.constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
	memcpy(MappedSubresource.pData, &toShader, sizeof(SEND_TO_VRAM));
	default_pipeline.context->Unmap(default_pipeline.constBuffer, NULL);

	default_pipeline.context->VSSetConstantBuffers(0, 1, &default_pipeline.constBuffer);
	default_pipeline.context->IASetVertexBuffers(0, 1, &default_pipeline.vertexBuffer, &stride, &offset);
	default_pipeline.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//this updates model too but im using the map/unmap
	 for (unsigned int i = 0; i < 100; i++)
	 { 
		 XMMATRIX newModelPer = XMMatrixTranspose(XMMatrixTranslation(upOne, 0, 0));
	 	XMStoreFloat4x4(&toShader.model, newModelPer);
		default_pipeline.context->Map(default_pipeline.constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
		memcpy(MappedSubresource.pData, &toShader, sizeof(SEND_TO_VRAM));
		default_pipeline.context->Unmap(default_pipeline.constBuffer, NULL);
	 upOne++;

	 }

	 default_pipeline.context->Draw(36, 0);
	 
	



	//	//This is everything I am doing to try and draw a second cube

	//SECOND STATIONARY CUBE
	XMMATRIX projection2 = XMMatrixTranspose(XMMatrixPerspectiveFovLH(FOV, aspRatio, 0.01f, 100.0f));
	XMStoreFloat4x4(&toShader2.projection, projection2);

	default_pipeline.context->Map(default_pipeline.constBuffer2, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
	memcpy(MappedSubresource.pData, &toShader2, sizeof(SEND_TO_VRAM));
	default_pipeline.context->Unmap(default_pipeline.constBuffer2, NULL);

	default_pipeline.context->VSSetConstantBuffers(0, 1, &default_pipeline.constBuffer2);
	default_pipeline.context->Draw(36, 0);


 	//	default_pipeline.context->Map(default_pipeline.constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
 	//	memcpy(MappedSubresource.pData, &toShader, sizeof(SEND_TO_VRAM));
 	//	default_pipeline.context->Unmap(default_pipeline.constBuffer, NULL);

	XMFLOAT3 passInColor = { 0.0f,0.0f,0.0f };
	float passInSize = 1.0f;
	XMFLOAT4 passInPosition = { 2.0f, 1.0f, 1.0f, 1.0f };
	
	//uncommenting this line currently breaks things
	//basicCube* testCube = new basicCube(&passInColor, &passInSize, &passInPosition);


	default_pipeline.m_pSwapChain->Present(1, 0);
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAY1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^  WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	initAll();

	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:

		break;
	case WM_KEYUP:
		//when i come back to this, i can update te constant buffer here so that it all works
		//and i dont have to worry about the order of precedence
		//updatesubresource or map/unmap

		break;
	case WM_LBUTTONDOWN:



		break;
	case WM_LBUTTONUP:

		break;
	case WM_RBUTTONDOWN:

		break;
	case WM_RBUTTONUP:

		break;
	case WM_MOUSEMOVE:

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

 

//normal is cross product between surface pos and 



