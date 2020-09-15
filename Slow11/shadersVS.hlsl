
cbuffer tcbuffer
{
	float4x4 MVP;
};
struct VertexIn
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float3 col : COLOR;
	float2 uv  :TEXCOORD;
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
	vOut.posH = mul(float4(vIn.pos,1.0),MVP);
	vOut.color = vIn.col;
	vOut.normal = vIn.nor;
	vOut.uv = vIn.uv;
	return vOut;
}