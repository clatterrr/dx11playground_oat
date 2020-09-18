struct VertexOut
{
	float4 posH : SV_POSITION; 
	float3 PosL : POSITION;
};

TextureCube gCubeMap:register(t0);
SamplerState g_sam:register(s0);

struct Light
{
	float4 Ambient;
	float4 Diffuse;
    float4 Position;
	float3 Direction;
	float Pad;
};

cbuffer ConstantBufferPerPixel
{
	Light light;
};

float4 PS(VertexOut pIn) : SV_Target
{
      return gCubeMap.Sample(g_sam, pIn.PosL.xyz);
}

