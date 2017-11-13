#include "stdafx.h"
#include "basicCube.h"

//what will be different about each cube?
//color
//size
//position 

//what will be same?
//shape
//

XMFLOAT3* cubeColor = new XMFLOAT3;
float* cubeSizeMultiplier = new float;
//XMFLOAT4* cubePosition = new XMFLOAT4;
SEND_TO_VRAM* cubeWorldData = nullptr;

//multiply each vert pos in the cube by the sizeOfCube factor
basicCube::basicCube(XMFLOAT3* color, float *sizeOfCube, XMFLOAT4* position)
{
	

	//setting cubes constant buffer to the default pipelines, will have to change the model matrix later, for now the other two will be the same
	cubeConstantBuffer = default_pipeline.constBuffer;
	//this is dangerously most probably the incorrect way of doing this
	cubeWorldData = (SEND_TO_VRAM*)cubeConstantBuffer;
	XMStoreFloat4x4(&cubeWorldData->model, XMMatrixIdentity());
	cubeDevice = default_pipeline.m_pDevice;
	cubeDeviceContext = default_pipeline.context;
	cubeResource = default_pipeline.resource;
	cubeSwapChain = default_pipeline.m_pSwapChain;
	cubeColor = color;
	cubeSizeMultiplier = sizeOfCube;
	//make sure to triple check this, I may need to revert to the other way, which was passing in a pointer to the triangles function and adding the position to each vert individually
	cubeWorldData->model._14 = position->x;
	cubeWorldData->model._24 = position->y;
	cubeWorldData->model._34 = position->z;
	cubeWorldData->model._44 = 1.0f;

	//cubePosition = position;
}




D3D11_SUBRESOURCE_DATA setupTriangles(XMFLOAT3& color = *cubeColor, float& size = *cubeSizeMultiplier)
{
	SIMPLE_VERTEX cubeVerts[8];

	XMFLOAT3 middleOfCube = { 0,0,0 };

	triangle cubeFaceTriangles[12];
	D3D11_SUBRESOURCE_DATA* totalTrianglesSrd;
	ZeroMemory(&totalTrianglesSrd, sizeof(totalTrianglesSrd));


	//this SHOULD set each vert to an index, so on overlap they will still go to their correct index
	for (unsigned int i = 0; i < 8; i++)
	{
	//	cubeVerts[i].index = i;
	}

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

	for (unsigned int j = 0; j < 8; j++)
	{
		cubeVerts[j].pos.x  ;
		cubeVerts[j].pos.y  ;
		cubeVerts[j].pos.z  ;

		if (size <= 0)
			break;
		else
		{
			cubeVerts[j].pos.x *= size;
			cubeVerts[j].pos.y *= size;
			cubeVerts[j].pos.z *= size;
		}
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
	if (&color != nullptr)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				cubeFaceTriangles[i].point[j].color = color;
			}
		}
	}
	totalTrianglesSrd->pSysMem = cubeFaceTriangles;

	return	*totalTrianglesSrd;
}



void basicCube::createBuffers()
{
	unsigned int stride = sizeof(SIMPLE_VERTEX);
	D3D11_BUFFER_DESC* cubeVertexBufferDesc = { 0 };
	cubeVertexBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
	cubeVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	cubeVertexBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cubeVertexBufferDesc->ByteWidth = sizeof(triangle) * 12;
	cubeVertexBufferDesc->MiscFlags = 0;

	cubeDevice->CreateBuffer(cubeVertexBufferDesc, &setupTriangles(), &cubeVertexBuffer);
	cubeDeviceContext->IASetVertexBuffers(0, 1, &cubeVertexBuffer, &stride, 0);

	D3D11_BUFFER_DESC* cubeIndexBufferDesc = { 0 };
	cubeIndexBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
	cubeIndexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
	cubeIndexBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//IN THE EVENT OF THINGS GOING WRONG, LOOK AT THIS BYTEWIDTH, MY MATH COULD BE OFF OR TOTALLY WRONG
	cubeIndexBufferDesc->ByteWidth = sizeof(cubeIndices) / sizeof(cubeIndices[0]);
	cubeIndexBufferDesc->MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA cubeIndexBufferSRD = { 0 };
	cubeIndexBufferSRD.pSysMem = cubeIndices;

	cubeDevice->CreateBuffer(cubeIndexBufferDesc, &cubeIndexBufferSRD, &cubeIndexBuffer);
	//cubeDeviceContext->IASetIndexBuffer(cubeIndexBuffer, 1, 0);

	cubeDeviceContext->VSSetConstantBuffers(0, 1, &cubeConstantBuffer);

	//still need an index buffer dont forget
	//WRITE NOTES TO YOURSELF IT WORKS LOOK AT THIS GUY REMINDING ME TO DO STUFF THANKS ME YOU ROCK
}

void basicCube::displayCube()
{

}

basicCube::~basicCube()
{
	delete cubeColor;
	delete cubeSizeMultiplier;
	delete cubeWorldData;
}
