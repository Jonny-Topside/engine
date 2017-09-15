#include "stdafx.h"
#include "DebugRenderer.h"


D3D11_SUBRESOURCE_DATA srd;

DebugRenderer::DebugRenderer()
{
	vertCount = 0;
	lineConstantBuffer = nullptr;
	lineConstantBuffDesc = nullptr;
	debugVertexBuffer = nullptr;
	stride = sizeof(SIMPLE_VERTEX);
	offset = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //map/unmap
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; //updatesubresource

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.ByteWidth = maxVerts * sizeof(SIMPLE_VERTEX);
	vertexBufferDesc.MiscFlags = 0;
	XMFLOAT3 tp = { 1.0, 1.0, 1.0 };
	XMFLOAT3 tc = { 1.0, 1.0, 1.0 };
	SIMPLE_VERTEX testdata = {tp,tc};
	//srd = { &testdata, 0, 0 };
	default_pipeline.m_pDevice->CreateBuffer(&vertexBufferDesc, nullptr, &debugVertexBuffer);

}


//void DebugRenderer::init()
//{
//	default_pipeline.m_pDevice->CreateBuffer(&vertexBufferDesc, &srd, &debugVertexBuffer);
//
//}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::add_debug_line(SIMPLE_VERTEX a, SIMPLE_VERTEX b, XMFLOAT3 color)
{
	line addedLine;
	//gpu side array is the vertex buffer 
	//cpu side array is an actual array that the verts will be added to
	//the flush is going to copy all the verts from the cpu array to the vertex buffer
	//if (vertCount < maxVerts)
	
		vertCount += 2;

		addedLine.point[0].pos = a.pos;
		addedLine.point[0].color = color;
		addedLine.point[1].pos = b.pos;
		addedLine.point[1].color = color;
		debugLines.push_back(addedLine.point[0]);
		debugLines.push_back(addedLine.point[1]);





	//else
		//vertCount = vertCount;

}

cube DebugRenderer::collisionLines(cube aabb)
{
	cube temp;
temp = aabb;
XMFLOAT3 col = { 1.0, 1.0, 1.0 };

add_debug_line(aabb.simps[0], aabb.simps[1], col);
add_debug_line(aabb.simps[0], aabb.simps[2], col);

add_debug_line(aabb.simps[0], aabb.simps[4], col);
add_debug_line(aabb.simps[1], aabb.simps[3], col);



add_debug_line(aabb.simps[1], aabb.simps[5], col);
add_debug_line(aabb.simps[2], aabb.simps[7], col);

add_debug_line(aabb.simps[2], aabb.simps[3], col);
add_debug_line(aabb.simps[5], aabb.simps[4], col);

add_debug_line(aabb.simps[5], aabb.simps[6], col);
add_debug_line(aabb.simps[7], aabb.simps[4], col);

add_debug_line(aabb.simps[6], aabb.simps[7], col);
add_debug_line(aabb.simps[6], aabb.simps[3], col);



	return temp;
}

void DebugRenderer::flush()
{


	srd = { debugLines.data(), 0, 0 };
	HRESULT h = default_pipeline.context->Map(debugVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedSubresources);
	memcpy(MappedSubresources.pData, debugLines.data(), sizeof(SIMPLE_VERTEX) * debugLines.size());
	default_pipeline.context->Unmap(debugVertexBuffer, NULL);

		//I should not have to create a buffer here
	//default_pipeline.m_pDevice->CreateBuffer(&vertexBufferDesc, &srd, &debugVertexBuffer);
	
	default_pipeline.context->IASetVertexBuffers(0, 1, &debugVertexBuffer, &stride, &offset);

	default_pipeline.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//how can i make it so that I dont have to create a new buffer and just use this?
	//default_pipeline.context->UpdateSubresource(default_pipeline.vertexBuffer, 0, NULL, &srd, 0, 0);
	

	default_pipeline.context->Draw(vertCount, 0);
	
	//int test = debugLines.size();
   //&debugLines.data();

	//Clearing the vector gives a different output, just one very long line
	//not clearing it will give correct output, currently its a triangle
	//THE SOLUTION TO THIS PROBLEM WAS THAT IN THE SRD I WAS USING &DEBUGLINES INSTEAD OF DEBUGLINES.DATA()

	debugLines.clear();
	vertCount = 0;
}


void DebugRenderer::clearVector()
{
	debugLines.clear();
}


