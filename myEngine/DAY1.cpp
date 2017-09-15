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
#include <DirectXMath.h>
#include <d3d11.h>
#include <strstream>
#include "PixelShader.csh"
#include "VertexShader.csh"
#define MAX_LOADSTRING 100
#define WIDTH 1080
#define HEIGHT 720
#define CYAN 0x00ffff
#define RED 0xff0000
#define MAGENTA 0xff00ff																																																																																	//Never let it be said that you couldn't do it Jonathan, you are capable of so much more than you think 4:58 AM
#define GREEN 0x00ff00
#define YELLOW 0xffff00
#define BLUE 0x0000ff
using namespace std;
using namespace DirectX;

// Global Variables:
float lookrot = 0.0f;
float rotation = 0;
float looker = 0;
float rotscale = 0.5f;
ID3D11InputLayout *float4_input_layout;
ID3D11Buffer* meshBuff = nullptr;
ID3D11Buffer* indexBuff;

HRESULT h;
float jonTimer = 0.0f;
SEND_TO_VRAM toShader;
SEND_TO_VRAM toShader2;
D3D11_SUBRESOURCE_DATA constantSrd;
D3D11_SUBRESOURCE_DATA constantSrd2;

cube testCube;
//view projection model



float aspRatio = WIDTH / HEIGHT;
float FOV = 70.0f * XM_PI / 180.0f;
float zNear = 0.1f;
float nearPlane = 0.1f;
float farPlane = 10.0f;

float zFar = 10.0f;


//FORWARD DECS
// Forward declarations of functions included in this code module:



//void TurnToalg();
XMFLOAT4X4 turnToFunction(XMFLOAT4X4 viewerMatrix, XMVECTOR targetPos, XMFLOAT4X4 cube, float speed);
XMFLOAT4X4 lookAtFunction(XMVECTOR eye, XMVECTOR at, XMVECTOR up, XMFLOAT4X4 cube);
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void calculateAABB(vector<SIMPLE_VERTEX> triangleInds);
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


		float timer2 = 0;
		if (GetAsyncKeyState(VK_UP))
		{
			timer2 += 0.04;
			//save translation matrix
			//multiply by jontimer
			//temp matrix load model matrix
			//third mat will be the mul of trans by transpose of temp mat
			XMMATRIX trans = XMMatrixTranspose(XMMatrixTranslation(0.0, 0, 1 * timer2 * 0.21f));
			XMMATRIX temp = XMLoadFloat4x4(&toShader2.model);
			XMMATRIX mulRes = XMMatrixMultiply(trans, temp);

			XMStoreFloat4x4(&toShader2.model, mulRes);



			//UpdateCube(0.01, 0.01, 0.01);

		}

		else if (GetAsyncKeyState(VK_DOWN))
		{
			//XMStoreFloat4x4(&toShader2.model, XMMatrixTranspose(XMMatrixTranslation(2, 0, -1)));

			timer2 += 0.04;
			//save translation matrix
			//multiply by jontimer
			//temp matrix load model matrix
			//third mat will be the mul of trans by transpose of temp mat
			XMMATRIX trans = XMMatrixTranspose(XMMatrixTranslation(0.0, 0, -1 * (1 * timer2 * 0.21f)));
			XMMATRIX temp = XMLoadFloat4x4(&toShader2.model);
			XMMATRIX mulRes = XMMatrixMultiply(trans, temp);

			XMStoreFloat4x4(&toShader2.model, mulRes);
		}

		else if (GetAsyncKeyState(VK_RIGHT))
		{
			XMStoreFloat4x4(&toShader2.model, XMMatrixRotationY(PI * 0.2f));
		}
		else if (GetAsyncKeyState(VK_LEFT))
		{
			XMStoreFloat4x4(&toShader2.model, XMMatrixRotationY(-1 * (PI * 0.2f)));

		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			XMVECTOR eye;

			//eye.m128_f32[0] = toShader.model._14;
			//eye.m128_f32[1] = toShader.model._24;
			//eye.m128_f32[2] = toShader.model._34;
			//eye.m128_f32[3] = toShader.model._44;
			//both work
			eye = XMVectorSet(toShader.model._14, toShader.model._24, toShader.model._34, toShader.model._44);
			XMVECTOR at;
			at = XMVectorSet(toShader2.model._14, toShader2.model._24, toShader2.model._34, toShader2.model._44);
			XMVECTOR up = { 0,1,0 };

			toShader2.model = lookAtFunction(eye, at, up, toShader2.model);
		}


		if (GetAsyncKeyState(VK_BACK))
		{
			if (looker == 0.0f)
				rotation = 0.0f;
			//float sp = 1;
			 XMVECTOR cube1;
			// cube1.m128_f32[0] = toShader.model._14;
			// cube1.m128_f32[1] = toShader.model._24;
			// cube1.m128_f32[2] = toShader.model._34;
			// cube1.m128_f32[3] = toShader.model._44;

	cube1 = XMVectorSet(toShader.model._14, toShader.model._24, toShader.model._34, toShader.model._44);
//			TurnToalg();
			toShader2.model = turnToFunction(toShader2.model, cube1, toShader2.model, 1);
		}


		drawCurrFrame();
		// In the future, do per frame/tick updates here...
	}
#ifndef NDEBUG 
	FreeConsole();
#endif 
	return 0;
}


//look at
//dont change entire position, freeze the one you have
//take the last column of the matrix
//new vector for new z
//subtract 

//calculating new x


void UpdateMouse(float deltaTime, float x, float y)
{
	static float roty = 0, rotx = 0;
	static float lookX = 0, lookY = 0, lookZ = 0;
	const float scaler = 5.0f;
	const float bump = deltaTime * scaler;
	roty -= x;
	rotx -= y;
	if (rotx >= XM_PI / 2)
		rotx = XM_PI / 2 - 0.001;
	if (rotx <= -XM_PI / 2)
		rotx = -XM_PI / 2 + 0.001;

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



	//XMStoreFloat4x4(&toShader.view, XMMatrixTranspose(XMLoadFloat4x4(&lookAtFunction(EyePosition, FocusPosition, UpDirection))));

	//XMStoreFloat4x4(&toShader2.view, XMMatrixTranspose(XMLoadFloat4x4(&lookAtFunction(EyePosition, FocusPosition, UpDirection))));



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

void initBuffer()
{
	triangle cubeFaceTriangles[12];
	D3D11_BUFFER_DESC vertexBufferDesc;

	SIMPLE_VERTEX cubeVerts[8];
	{
		//1
		cubeVerts[0].pos.x = -0.25f;
		cubeVerts[0].pos.y = -0.25f;		//BOTTOM LEFT FRONT 1
		cubeVerts[0].pos.z = -0.25f;
		//2			    	  
		cubeVerts[1].pos.x = -0.25f;
		cubeVerts[1].pos.y = -0.25f;		//BOTTOM LEFT BACK 2
		cubeVerts[1].pos.z = 0.25f;
		//3			    	  
		cubeVerts[2].pos.x = 0.25f;
		cubeVerts[2].pos.y = -0.25f;		//BOTTOM RIGHT FRONT 3
		cubeVerts[2].pos.z = -0.25f;
		//4		    		  
		cubeVerts[3].pos.x = 0.25f;
		cubeVerts[3].pos.y = -0.25f;		//BOTTOM RIGHT BACK 4
		cubeVerts[3].pos.z = 0.25f;
		//5			    	  
		cubeVerts[4].pos.x = -0.25f;
		cubeVerts[4].pos.y = 0.25f;		// TOP LEFT FRONT 5
		cubeVerts[4].pos.z = -0.25f;
		//6		    		  
		cubeVerts[5].pos.x = -0.25f;
		cubeVerts[5].pos.y = 0.25f;		//TOP LEFT BACK 6
		cubeVerts[5].pos.z = 0.25f;
		//7		    		  
		cubeVerts[6].pos.x = 0.25f;
		cubeVerts[6].pos.y = 0.25f;		// TOP RIGHT BACK 7 
		cubeVerts[6].pos.z = 0.25f;
		//8	    			  
		cubeVerts[7].pos.x = 0.25f;
		cubeVerts[7].pos.y = 0.25f;		//TOP RIGHT FRONT 8
		cubeVerts[7].pos.z = -0.25f;
	}

	{

		//FACE 1 TRIANGLE 1 -Z
		//1
		cubeFaceTriangles[0].point[0] = cubeVerts[0];
		cubeFaceTriangles[0].point[1] = cubeVerts[4];
		cubeFaceTriangles[0].point[2] = cubeVerts[7];
		//2											
		cubeFaceTriangles[1].point[0] = cubeVerts[0];
		cubeFaceTriangles[1].point[1] = cubeVerts[7];
		cubeFaceTriangles[1].point[2] = cubeVerts[2];
		for (unsigned int i = 0; i < 2; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 1.0f, 1.0f, 0.0f }; //yellow
			}
		}


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
		}
		//FACE 5 TRIANGLE 9 +Y
		//9
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
	//second stuff


	triangle cubeFaceTriangles2[12];
	D3D11_BUFFER_DESC vertexBufferDesc2;

	{
		//1
		cubeVerts[0].pos.x = -0.25f;
		cubeVerts[0].pos.y = -0.25f;		//BOTTOM LEFT FRONT 1
		cubeVerts[0].pos.z = -0.25f;
		//2			    	  
		cubeVerts[1].pos.x = -0.25f;
		cubeVerts[1].pos.y = -0.25f;		//BOTTOM LEFT BACK 2
		cubeVerts[1].pos.z = 0.25f;
		//3			    	  
		cubeVerts[2].pos.x = 0.25f;
		cubeVerts[2].pos.y = -0.25f;		//BOTTOM RIGHT FRONT 3
		cubeVerts[2].pos.z = -0.25f;
		//4		    		  
		cubeVerts[3].pos.x = 0.25f;
		cubeVerts[3].pos.y = -0.25f;		//BOTTOM RIGHT BACK 4
		cubeVerts[3].pos.z = 0.25f;
		//5			    	  
		cubeVerts[4].pos.x = -0.25f;
		cubeVerts[4].pos.y = 0.25f;		// TOP LEFT FRONT 5
		cubeVerts[4].pos.z = -0.25f;
		//6		    		  
		cubeVerts[5].pos.x = -0.25f;
		cubeVerts[5].pos.y = 0.25f;		//TOP LEFT BACK 6
		cubeVerts[5].pos.z = 0.25f;
		//7		    		  
		cubeVerts[6].pos.x = 0.25f;
		cubeVerts[6].pos.y = 0.25f;		// TOP RIGHT BACK 7 
		cubeVerts[6].pos.z = 0.25f;
		//8	    			  
		cubeVerts[7].pos.x = 0.25f;
		cubeVerts[7].pos.y = 0.25f;		//TOP RIGHT FRONT 8
		cubeVerts[7].pos.z = -0.25f;
	}

	for (unsigned int i = 0; i < 8; i++)
	{
		testCube.simps[i].pos.x = cubeVerts[i].pos.x + 0.25;
		testCube.simps[i].pos.y = cubeVerts[i].pos.y + 1.25;
		testCube.simps[i].pos.z = cubeVerts[i].pos.z + 0.25;

	}

	{

		//FACE 1 TRIANGLE 1 -Z
		//1
		cubeFaceTriangles[0].point[0] = cubeVerts[0];
		cubeFaceTriangles[0].point[1] = cubeVerts[4];
		cubeFaceTriangles[0].point[2] = cubeVerts[7];
		//2											
		cubeFaceTriangles[1].point[0] = cubeVerts[0];
		cubeFaceTriangles[1].point[1] = cubeVerts[7];
		cubeFaceTriangles[1].point[2] = cubeVerts[2];
		for (unsigned int i = 0; i < 2; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = { 1.0f, 1.0f, 0.0f }; //yellow
			}
		}


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
		}
		//FACE 5 TRIANGLE 9 +Y
		//9
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

	vertexBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc2.ByteWidth = sizeof(triangle) * 12;
	vertexBufferDesc2.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA srd2 = { cubeFaceTriangles, 0, 0 };
	default_pipeline.drawnBuffDesc2 = &vertexBufferDesc2;
	default_pipeline.m_pDevice->CreateBuffer(default_pipeline.drawnBuffDesc2, &srd2, &default_pipeline.vertexBuffer2);







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


//FOR THIS FUNCTION, RETURNS A NEW MATRIX MADE UP OF A EYEPOS, FOCUSPOS, AND UP
//DIRECTION = EYE - AT

//XMFLOAT4X4 lookAtFunction(TVECTOR eye, TVECTOR at, TVECTOR up)
XMFLOAT4X4 lookAtFunction(XMVECTOR eye, XMVECTOR at, XMVECTOR up, XMFLOAT4X4 cube)
{
	//position does not change save it first then put it back in
	//toShader2.model

	XMFLOAT4X4 output;

	//TVECTOR zAxis = Vector_Normalize(Vector_Sub(eye, at));
	XMVECTOR zAxis = XMVectorSubtract(eye, at);
	//zAxis.m128_f32[0] = 1;
	//TVECTOR xAxis = Vector_Normalize(Vector_Cross(up, zAxis));
	//TVECTOR yAxis = Vector_Normalize(Vector_Cross(zAxis, xAxis));
	//output._11 = xAxis.x; 
	XMStoreFloat4x4(&output, XMMatrixLookToLH(eye, zAxis, up));
	output._14 = cube._14;
	output._24 = cube._24;
	output._34 = cube._34;
	output._44 = cube._44;

	//return XMMatrixLookToLH(eye, zAxis, up);
	return output;
}

 
XMFLOAT4X4 turnToFunction(XMFLOAT4X4 viewerMatrix, XMVECTOR targetPos, XMFLOAT4X4 cube, float speed)
{
	XMVECTOR temp;
	float getDot;
	XMFLOAT4X4 fin;

	XMFLOAT3 movingCube = XMFLOAT3(toShader2.model._14, toShader2.model._24, toShader2.model._34);
	XMFLOAT3 tempSub = movingCube;
	XMFLOAT3 stillCube = XMFLOAT3(toShader.model._14, toShader.model._24, toShader.model._34);
	 	tempSub.x = stillCube.x - movingCube.x;
	 	tempSub.y = stillCube.y - movingCube.y;
	 	tempSub.z = stillCube.z - movingCube.z;

		XMFLOAT3 xAxisOfStillCube = { viewerMatrix._11, viewerMatrix._21, viewerMatrix._31 };
			float dotResult = (xAxisOfStillCube.x * tempSub.x) + (xAxisOfStillCube.y * tempSub.y) + (xAxisOfStillCube.z * tempSub.z);
			rotation += dotResult;
	//subtract the position of both objs
//	XMVECTOR tempPos1 = XMVectorSet(viewerMatrix._11, viewerMatrix._21, viewerMatrix._31, viewerMatrix._41);
	
	//XMVECTOR tempPos4 = XMVectorSet(viewerMatrix._14, viewerMatrix._24, viewerMatrix._34, viewerMatrix._44);
	//temp = XMVectorSubtract(tempPos4, targetPos);
	//temp = XMVector3Normalize(temp);
	//dot product between the sub and 11,21,31 of the thing meant to turn to
	//XMVECTOR result = XMVector4Dot(temp, targetPos);
	//rotation += result.m128_f32[0] + result.m128_f32[1] + result.m128_f32[2] + result.m128_f32[3];
	//getDot = result.m128_f32[0] + result.m128_f32[1] + result.m128_f32[2] + result.m128_f32[3];

	//result 
	//freeze position again
	//convert to radians
	//XMStoreFloat4x4(&fin, XMMatrixMultiply(XMLoadFloat4x4(&viewerMatrix), XMMatrixRotationY((XMConvertToRadians(speed) * 0.4))));

	//XMStoreFloat4x4(&fin, XMMatrixMultiply(XMLoadFloat4x4(&viewerMatrix), XMMatrixRotationY(speed* 0.1)));

	XMStoreFloat4x4(&fin, XMMatrixRotationY(XMConvertToRadians(-rotation) *speed));

	
	looker = dotResult;
	//XMVECTOR tempPos2 = XMVectorSet(viewerMatrix._12, viewerMatrix._22, viewerMatrix._32, viewerMatrix._42);
	//XMVECTOR tempPos3 = XMVectorSet(viewerMatrix._13, viewerMatrix._23, viewerMatrix._33, viewerMatrix._43);
	//
	////XMStoreFloat4x4(&fin,  XMLoadFloat4x4(&viewerMatrix) );
	//
	//XMVECTOR finTempPos1 = XMVectorSet(fin._11, fin._21, fin._31, fin._41);
	//XMVECTOR finTempPos2 = XMVectorSet(fin._12, fin._22, fin._32, fin._42);
	//XMVECTOR finTempPos3 = XMVectorSet(fin._13, fin._23, fin._33, fin._43);
	//XMVECTOR finTempPos4 = XMVectorSet(fin._14, fin._24, fin._34, fin._44);
	//check if the rotation == to the subtraction
	//CHECK IF SECOND ROW WITH 2S IS THE SAME
	//if (finTempPos2.m128_f32[0] == tempPos2.m128_f32[0] && finTempPos2.m128_f32[1] == tempPos2.m128_f32[1] && finTempPos2.m128_f32[2] == tempPos2.m128_f32[2] && finTempPos2.m128_f32[3] == tempPos2.m128_f32[3] )
	//{
	//	XMStoreFloat4x4(&fin, XMLoadFloat4x4(&fin));
	//}
	//if (finTempPos1.m128_f32[0] == tempPos1.m128_f32[0] && finTempPos1.m128_f32[1] == tempPos1.m128_f32[1] && finTempPos1.m128_f32[2] == tempPos1.m128_f32[2] && finTempPos1.m128_f32[3] == tempPos1.m128_f32[3])
	//{
	//	XMStoreFloat4x4(&fin, XMLoadFloat4x4(&fin));
	//}
	//if (finTempPos3.m128_f32[0] == tempPos3.m128_f32[0] && finTempPos3.m128_f32[1] == tempPos3.m128_f32[1] && finTempPos3.m128_f32[2] == tempPos3.m128_f32[2] && finTempPos3.m128_f32[3] == tempPos3.m128_f32[3])
	//{
	//	XMStoreFloat4x4(&fin, XMLoadFloat4x4(&fin));
	//}
	// if (finTempPos4.m128_f32[0] == tempPos4.m128_f32[0] && finTempPos4.m128_f32[1] == tempPos4.m128_f32[1] && finTempPos4.m128_f32[2] == tempPos4.m128_f32[2] && finTempPos4.m128_f32[3] == tempPos4.m128_f32[3])
	// {
	// 	XMStoreFloat4x4(&fin, XMLoadFloat4x4(&fin));
	// }
	//XMStoreFloat4x4(&fin, XMMatrixMultiplyTranspose(result);
	//XMMatrixRotationNormal(result, speed);

	fin._14 = cube._14;
	fin._24 = cube._24;
	fin._34 = cube._34;
	fin._44 = cube._44;

	//  looker = result.m128_f32[0] + result.m128_f32[1] + result.m128_f32[2] + result.m128_f32[3];

	return fin;
}

XMFLOAT4X4 mouseLookFunction(XMFLOAT4X4 m, float dx, float dy)
{	
	XMFLOAT4X4 tempFloat = m;
	XMStoreFloat4x4(&tempFloat, XMMatrixMultiply(XMMatrixRotationX(dy), XMLoadFloat4x4(&tempFloat)));
	XMStoreFloat4x4(&tempFloat, XMMatrixMultiply(XMMatrixRotationY(dx), XMLoadFloat4x4(&tempFloat)));
	return tempFloat;
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


	D3D11_INPUT_ELEMENT_DESC vLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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

	const XMVECTORF32	EyePosition = { -1.45, -50, -50, 1.0f };
	const XMVECTORF32 FocusPosition = { cos(10)*cos(10) + 1, sin(10) + 1, sin(10)*cos(10) + 1, 0.f };
	const XMVECTORF32	UpDirection = { .0f, 1.f, 0.f, 0.f };


	XMMATRIX LookAt = XMMatrixLookAtLH(EyePosition, FocusPosition, UpDirection);

	const XMVECTORF32	EyePosition2 = { -1.45, -50, -50, 1.0f };
	const XMVECTORF32 FocusPosition2 = { cos(10)*cos(10) + 1, sin(10) + 1, sin(10)*cos(10) + 1, 0.f };
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
	initBuffer();
	setPipeline(&default_pipeline);

}



void drawCurrFrame()
{
	DebugRenderer debugRender;// = new DebugRenderer;
	
	jonTimer += 0.11f;
	const float newColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	unsigned int offset = 0;
	unsigned int stride = sizeof(SIMPLE_VERTEX);



	XMMATRIX projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(FOV, aspRatio, 0.01f, 100.0f));
	XMStoreFloat4x4(&toShader.projection, projection);

	////it wont work with xmfloat4;
	//XMFLOAT4 color;
	//DRAW ROTATING CUBE
	default_pipeline.context->ClearRenderTargetView(default_pipeline.render_target, newColor);
	default_pipeline.context->ClearDepthStencilView(default_pipeline.depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	//changed
	XMStoreFloat4x4(&toShader.model, XMMatrixMultiplyTranspose(XMLoadFloat4x4(&toShader.model), XMMatrixMultiply(XMMatrixRotationX(.1*jonTimer), XMMatrixRotationZ(.1*jonTimer))));
	D3D11_MAPPED_SUBRESOURCE MappedSubresource;
	
	default_pipeline.context->Map(default_pipeline.constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
	memcpy(MappedSubresource.pData, &toShader, sizeof(SEND_TO_VRAM));
	default_pipeline.context->Unmap(default_pipeline.constBuffer, NULL);

	default_pipeline.context->VSSetConstantBuffers(0, 1, &default_pipeline.constBuffer);
	default_pipeline.context->IASetVertexBuffers(0, 1, &default_pipeline.vertexBuffer, &stride, &offset);
	default_pipeline.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	default_pipeline.context->UpdateSubresource(default_pipeline.constBuffer, 0, NULL, &toShader, 0, 0);
	default_pipeline.context->Draw(36, 0);


	//	//This is everything I am doing to try and draw a second cube
	//
	//	//could I just use an identity matrix? Yes
	//	//here, maybe i can do an xmmatrixtranslatetranspose on an offset YES
	//SECOND STATIONARY CUBE
	XMMATRIX projection2 = XMMatrixTranspose(XMMatrixPerspectiveFovLH(FOV, aspRatio, 0.01f, 100.0f));
	XMStoreFloat4x4(&toShader2.projection, projection2);

	default_pipeline.context->Map(default_pipeline.constBuffer2, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
	memcpy(MappedSubresource.pData, &toShader2, sizeof(SEND_TO_VRAM));
	default_pipeline.context->Unmap(default_pipeline.constBuffer2, NULL);

	default_pipeline.context->VSSetConstantBuffers(0, 1, &default_pipeline.constBuffer2);
	default_pipeline.context->Draw(36, 0);

	//DEBUG RENDERER THINGS
	SIMPLE_VERTEX start1, start2, b, c, d, b2, c2, d2;
	XMFLOAT3 colorForX = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 colorForY = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 colorForZ = { 0.0f, 0.0f, 1.0f };
	{
		start1.pos.x = 0.0f;
		start1.pos.y = 0.0f;
		start1.pos.z = 0.0f;

		start2.pos.x = 0.0f - 2;
		start2.pos.y = 0.0;
		start2.pos.z = 0.0;
		b2.pos.x = 0.7f - 2;//x
		b2.pos.y = 0.0f;
		b2.pos.z = 0.0f;
		c2.pos.x = 0.0f - 2;//y
		c2.pos.y = 0.7f;
		c2.pos.z = 0.0f;
		d2.pos.x = 0.0f - 2;//z
		d2.pos.y = 0.0f;
		d2.pos.z = 0.7f;
		b.pos.x = 0.7f;//x
		b.pos.y = 0.0f;
		b.pos.z = 0.0f;
		c.pos.x = 0.0f;//y
		c.pos.y = 0.7f;
		c.pos.z = 0.0f;
		d.pos.x = 0.0f;//z
		d.pos.y = 0.0f;
		d.pos.z = 0.7f;
	}
	XMStoreFloat4x4(&toShader.model, XMMatrixIdentity());

	default_pipeline.context->Map(default_pipeline.constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresource);
	memcpy(MappedSubresource.pData, &toShader, sizeof(SEND_TO_VRAM));
	default_pipeline.context->Unmap(default_pipeline.constBuffer, NULL);

	//console project 2
	//io of more information
	//specular diffuse values 
	//I will need to make a struct of my own material, like last month but for lights or soemthing
	//serialize means write to file (big binary blob)
	debugRender.add_debug_line(start1, b, colorForX);
	debugRender.add_debug_line(start1, c, colorForY);
	debugRender.add_debug_line(start1, d, colorForZ);
	debugRender.add_debug_line(start2, b2, colorForX);
	debugRender.add_debug_line(start2, c2, colorForY);
	debugRender.add_debug_line(start2, d2, colorForZ);
	//
	//	//RIGHT NOW THE INDEX BUFFER WORKS

	//	//FBX THINGS
	default_pipeline.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer* mageConstantBuffer = nullptr;
	fbx::fbxFunctions::setup("BattleMage.fbx");
	simpMesh terrain = fbx::fbxFunctions::getTheMesh();
	fbx::fbxFunctions::writeMesh(terrain);
	//start of reading mesh to render
	terrain = fbx::fbxFunctions::readMesh("projectMesh.bin"); 
	cout << terrain.vertData.data();
	stride = sizeof(SIMPLE_VERT);

	D3D11_BUFFER_DESC mageBufferDesc;
	ZeroMemory(&mageBufferDesc, sizeof(mageBufferDesc));
	mageBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mageBufferDesc.ByteWidth = sizeof(SIMPLE_VERT)* terrain.vertData.size();
	mageBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mageBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	default_pipeline.m_pDevice->CreateBuffer(&mageBufferDesc, NULL, &meshBuff); 

	D3D11_MAPPED_SUBRESOURCE mappedMeshSRD;
	default_pipeline.context->Map(meshBuff, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedMeshSRD);
	memcpy(mappedMeshSRD.pData, terrain.vertData.data(), sizeof(SIMPLE_VERT)* terrain.vertData.size());
	default_pipeline.context->Unmap(meshBuff, NULL);

	default_pipeline.context->IASetInputLayout(float4_input_layout);
	default_pipeline.context->IASetVertexBuffers(0, 1, &meshBuff, &stride, &offset);
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0,0,0 };
	indexBufferData.pSysMem = terrain.indices.data();
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * terrain.indices.size(), D3D11_BIND_INDEX_BUFFER);
	default_pipeline.m_pDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuff);
	default_pipeline.context->IASetIndexBuffer(indexBuff, DXGI_FORMAT_R32_UINT, 0);
	int sze = terrain.indices.size();
	default_pipeline.context->DrawIndexed(terrain.indices.size(), 0, 0);
	
	vector<SIMPLE_VERTEX> inds;

	calculateAABB(inds);
	debugRender.flush();

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
	bitset<4> newBitset;
	bool isKeyBeingPressed = false;
	bool isMouseMoving = false;
	bool isMouseRightBeingPressed = false;
	bool isMouseLeftBeingPressed = false;
	newBitset[0] = 1;
	newBitset[1] = false;
	newBitset[2] = 34;
	switch (message)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		isKeyBeingPressed = true;
		if (isKeyBeingPressed == true && wParam == VK_RIGHT)
		{
			Matrix_Create_Rotation_Y(XMConvertToRadians(0.2f * jonTimer));
		}
		break;
	case WM_KEYUP:
		//when i come back to this, i can update te constant buffer here so that it all works
		//and i dont have to worry about the order of precedence
		//updatesubresource or map/unmap
		isKeyBeingPressed = false;
		break;
	case WM_LBUTTONDOWN:


		isMouseLeftBeingPressed = true;
		break;
	case WM_LBUTTONUP:
		isMouseLeftBeingPressed = false;
		break;
	case WM_RBUTTONDOWN:
		isMouseRightBeingPressed = true;
		break;
	case WM_RBUTTONUP:
		isMouseRightBeingPressed = false;
		break;
	case WM_MOUSEMOVE:
		if (isMouseRightBeingPressed && isKeyBeingPressed && wParam == VK_LEFT)
		{
			//THIS PROBABLY ISNT HOW I SHOULD BE DOING THIS
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void calculateAABB(vector<SIMPLE_VERTEX> triangleInds)
{
	//takes in the 8 points
	//draw a line from 0-1, 1-5, 4-5 etc.





//	default_pipeline.context->Draw(triangleInds.size() * 2, 0);


	//debugRender.collisionLines(testCube);



}

//normal is cross product between surface pos and 



