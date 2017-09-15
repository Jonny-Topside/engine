//struct INPUT_VERTEX
//{
//	float3 pos : POSITION;
//
//};
//
//struct OUTPUT_VERTEX
//{
//	float3 color : COLOR;
//	float4 pos : SV_POSITION;
//};
//
//
//cbuffer modelViewProjectionConstantBuffer : register(b0)
//{
//	matrix model;
//	matrix view;
//	matrix projection;
//}
//
//
//OUTPUT_VERTEX main(INPUT_VERTEX input)
//{
//	OUTPUT_VERTEX output;
//
//	float4 pos = float4(input.pos, 1.0f);
//
//	pos = mul(pos, model);
//	pos = mul(pos, view);
//	pos = mul(pos, projection);
//	output.pos = pos;
//
//	output.color = float3(1.0f, 0.0f, 1.0f);
//
//	return output;
//}


struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

struct OUTPUT_VERTEX
{
	//switching these gives a completely different affect to the cube colors what does it mean?
	//order matters, 
	float4 pos : SV_POSITION;
	float3 colorOut : COLOR0;
};

cbuffer modelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
}


OUTPUT_VERTEX main(VertexShaderInput input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	float4 pos = float4(input.pos, 1.0f);
	 
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	//pos.w = 1;
	
	output.colorOut = input.color;


	output.pos = pos;

	return output;
}
