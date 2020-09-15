struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 color: COLOR;
	float3 normal : NORMAL;
	float2 uv   : TEXCOORD;
};

Texture2D g_Tex:register(t0);
SamplerState g_sam:register(s0);

struct Light
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

cbuffer ConstantBufferPerPixel
{
	Light light;
};
/*
void ComputeDirectionLight(DirectionLight L, float normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -L.Direction;
	ambient = 0.8f * L.Ambient;
	float diffuseFactor  dot(lightVec, normal);
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), 0.8f);
		diffuse = diffuseFactor * 0.8f * L.Diffuse;
		spec = specFactor * 0.8f * L.Specular;
	}
}*/

float4 PS(VertexOut pIn) : SV_Target
{
	//pIn.normal = normalize(pIn.normal);
    //float3 toEyeW = normalize(float3(0.0f, 0.0f, -1.0f) - pIn.posH);
    

    

	float4 texColor = g_Tex.Sample(g_sam,pIn.uv);
	return  texColor + light.Ambient;
	//return float4(1.0f,1.0f,0.0f,1.0f);
    //return pIn.color;
}

