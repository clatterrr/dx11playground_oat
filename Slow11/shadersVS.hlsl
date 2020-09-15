
cbuffer tcbuffer
{
	float4x4 MVP;
};
struct VertexIn
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float3 col : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 color:COLOR;
};


VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.pos,1.0),MVP);
	vOut.color = vIn.col;
	return vOut;
}