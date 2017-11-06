
cbuffer lights : register(b0)
{

	float3 diffuse;
	float3 emissive;
	float3 ambient;
	float3 specular;
	float3 transparency;
	float3 reflection;
}

struct lightCreation
{
	float4 diffuse;
	float4 coneDir;
	float4 conePos;
	float4 ambient;
	float4 color;
	float range;
	float4 dir;

	float4 pos;
	float3 att;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 wPos : WORLDPOSITION;
};

Texture2D mageDiffuseText : register(t0);
SamplerState mageDiffuseSamplerState : register(s0);

Texture2D mageEmissiveText : register(t1);
SamplerState mageEmissiveSamplerState : register(s1);

Texture2D mageSpecularText : register(t2);
SamplerState mageSpecularSamplerState : register(s2);
//these come in here to the pixel shader based on which pixel shader it currently is

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4  diffuseTex;
	float4  emissiveTex;
	float4  specularTex;
	float4 finalDiffuse;
	float4 finalEmissive;
	float4 finalSpecular;
	float4 colorToReturn;
	float4 directionReturnVal;
	float4 pointReturnVal;
	//multiply texture by final light color calculation
	float4 spotLightResult;
	lightCreation lighter;

	//sample each texture individually then add the colors up for the final color
	diffuseTex = mageDiffuseText.Sample(mageDiffuseSamplerState, input.uv);
	emissiveTex = mageEmissiveText.Sample(mageEmissiveSamplerState, input.uv);
	specularTex = mageSpecularText.Sample(mageSpecularSamplerState, input.uv);
	//put in a point light/spot/dir light
	// finalDiffuse = diffuseTex * float4(diffuse.xyz, 1.0f);
	finalDiffuse = diffuseTex;
	finalEmissive = emissiveTex;//* (emissive);
	finalSpecular = specularTex;//* (specular) ;

//	 colorToReturn = (finalDiffuse * float4(diffuse.xyz, 1.0f)) + (finalEmissive * float4(emissive.xyz, 1.0f))  + (finalSpecular * float4(specular.xyz, 1.0f));
	colorToReturn = (finalDiffuse  ) + (finalEmissive  ) + (finalSpecular  );

	lighter.dir = float4(-1.0f, -0.33f, -0.33f, 1.0f);
	 
	//lighter.ambient = colorToReturn * float4(ambient.xyz, 1.0f);
	lighter.ambient = colorToReturn * 0.4f;

	float surfaceRat = 0;
	float lightRat = 0;
	float coneRat = 0;
	lighter.coneDir = float4(1.0f, 10.0f, 0.0f, 1);

	//DIRECTIONAL LIGHT : BLUE
	lighter.color = float4(1, 1, 1, 1.0f);
	lightRat = clamp(dot(-lighter.dir.xyz, input.normal), 0, 1);
	directionReturnVal = lightRat * lighter.color * colorToReturn;
	//POINT LIGHT : RED
	lighter.color = float4(0, 0, 0.0f, 1.0f);
	lighter.pos = float4(0.0F, 1.0f, 0.0f, 1);// *sin(180);
	lighter.dir = normalize(lighter.pos - input.wPos);
	lightRat = saturate(dot(lighter.dir.xyz, input.normal.xyz));
	float attenuation = 1.0f - clamp(length(lighter.pos - input.wPos) / 15.1f, 0, 1);
	pointReturnVal = lightRat * lighter.color * colorToReturn * attenuation;
	//SPOTLIGHT : YELLOW
	lighter.conePos = float4(0.0f, 1.7f, -0.05f, 1.0f);
	lighter.color = float4(0, 0, 0.0f, 1.0f);
	lighter.dir = normalize(lighter.conePos - input.wPos);
	surfaceRat = clamp(dot(-lighter.dir, lighter.coneDir), 0, 1);
	float spotFactor = (surfaceRat > coneRat) ? 1 : 0;
	lightRat = clamp(dot(lighter.dir, input.normal), 0, 1);
	spotLightResult = spotFactor * lightRat * lighter.color * colorToReturn;

	//return colorToReturn;
	float4 saturn = saturate(spotLightResult + (directionReturnVal + pointReturnVal) + lighter.ambient);
	
	finalDiffuse *= saturn;
	finalEmissive *= saturn;
	finalSpecular *= saturn;

	return saturate(finalDiffuse + finalEmissive + finalSpecular);
	//float3 finalLight = ambient + diffuse + specular + emissive + input.normal; 

	



}