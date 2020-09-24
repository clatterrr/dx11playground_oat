
cbuffer tcbuffer
{
	float4x4 MVP;
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProj;
	float4 gEyePos;
};
struct VertexIn
{
	float3 pos : POSITION;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 color: COLOR;
	float3 normal : NORMAL;
	float2 uv   : TEXCOORD;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.pos, 1.0), MVP);
	//vOut.color = vIn.col;
	//vOut.normal = vIn.nor;
	//vOut.uv = vIn.uv;
	vOut.color = float3(1.0f, 1.0f, 1.0f);
	vOut.normal = float3(1.0f, 1.0f, 1.0f);
	vOut.uv = float2(1.0f, 0.0f);
	return vOut;
}