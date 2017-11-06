

struct VertexShaderInput
{
	float4 pos : POSITION;
	//this was giving problems right here
	float4 color : COLOR0;
	//float2 uv : UV;
	//float3 normal : NORMAL;
};

struct OUTPUT_VERTEX
{
	//switching these gives a completely different affect to the cube colors what does it mean?
	//order matters, 
	float4 pos : SV_POSITION;
	float4 colorOut : COLOR0;
	//float2 uv : UV;
	//float3 normal : NORMAL;
	//float4 wPos : WORLDPOSITION;
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
	float4 pos = float4(input.pos);
	 
	//if (input.indices.x >= 0)
	//{
	//	output.pos = mul(joints[input.indices.x], input.pos) * input.weights.x;
	//	if (input.indices.y >= 0)
	//	{
	//		output.pos += mul(joints[input.indices.y], input.position) * input.weights.y;
	//	}
	//	if (input.indices.z >= 0)
	//	{
	//		output.pos += mul(joints[input.indices.z], input.pos) * input.weights.z;
	//	}
	//	if (input.indices.w >= 0)
	//	{
	//		output.pos = mul(joints[input.indices.w], input.pos) * input.weights.w;
	//	}
	//}
	//else
	//{
	//	output.pos = input.pos;
	//}

	pos = mul(pos, model);
	//output.wPos = pos;
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	output.colorOut = input.color;
	//output.uv = input.uv;
	//output.normal = mul(input.normal, model);

	return output;
}
